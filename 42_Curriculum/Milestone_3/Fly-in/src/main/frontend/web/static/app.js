/**
 * Fly-in Simulator: front-end map viewer and simulation player.
 *
 * Rule of the file: event handlers only change `state`. A single
 * requestAnimationFrame loop (frame -> update_simulation -> render) is the
 * only code that draws.
 *
 * Data files (paths relative to index.html)
 *   parsed_maps/index.json                  [{name, difficulty, path}, ...]
 *   parsed_maps/<difficulty>/<name>.json    {hubs: [...], connections: [...]}
 *   parsed_maps/<difficulty>/<name>_sim.json
 *       {map, drones, turns: [{turn_nb, turn_data: [
 *           {drone, from, to, in_transit}, ...]}, ...]}
 *
 * Flow
 *   main() -> loads the index -> show_map() loads the map and its
 *   simulation file into `state`. frame() runs forever: it advances the
 *   simulation clock (only while running) and renders the whole canvas
 *   (connections, hubs, drones) every frame.
 *
 * Simulation time
 *   sim.elapsed is in milliseconds. turn_float = elapsed / TURN_MS: the
 *   integer part is the current turn, the fraction is the progress inside
 *   it. Drone positions are computed from turn_float, never moved
 *   incrementally.
 */

/* || ------- Constants ------- */

const HUB_RADIUS = 20;
const DRONE_RADIUS = 8;         // fallback circle, used while the image is missing
const DRONE_IMAGE = "assets/Drone.png";
const DRONE_SIZE = 100;          // drawn width in pixels (height keeps the ratio)
const HUD_FONT_SIZE = 16;       // multiple of 8: sharp with Press Start 2P
const HUD_PAD = 10;             // space between the stats panel border and text
const HUD_MARGIN = 10;          // space between the panel and the canvas edge
const TURN_MS = 600;            // on-screen duration of one simulation turn
const MAX_DELTA_MS = 100;       // cap per frame, avoids jumps after a hidden tab
const RAINBOW_STEP_MS = 100;    // the rainbow hue changes 10 degrees every step
const HIDE_ON_ARRIVAL = true;   // hide a drone once it reached its last hub

/* || ------- State ------- */

const view = { min_x: 0, min_y: 0, scale: 1, off_x: 0, off_y: 0 };

const state = {
    groups: new Map(),   // difficulty -> list of index entries
    difficulty: null,    // currently selected difficulty
    position: 0,         // position inside the selected difficulty's list
    request: 0,          // id of the latest map request (detects stale responses)
    map: null,           // {hubs, connections, by_name} of the shown map
    sim: new_sim(),      // simulation of the shown map
};

/**
 * Create an empty simulation state.
 *
 * status: "idle" | "running" | "paused" | "finished"
 *
 * @returns {Object} Fresh simulation state.
 */
function new_sim() {
    return {
        timeline: null,     // {start, segments} built from the sim file
        drone_count: 0,
        total_turns: 0,
        status: "idle",
        elapsed: 0,         // simulated milliseconds
        last_frame: null,   // timestamp of the previous frame
    };
}

/* || ------- DOM references ------- */

/** @type {HTMLCanvasElement} */
const canvas = document.getElementById("simulation");
const ctx = canvas.getContext("2d");
const difficulty_select = document.getElementById("difficultySelect");
const prev_btn = document.getElementById("prevBtn");
const next_btn = document.getElementById("nextBtn");
const map_name = document.getElementById("mapName");
const start_btn = document.getElementById("startBtn");
const pause_btn = document.getElementById("pauseBtn");
const restart_btn = document.getElementById("restartBtn");

/* Loaded once, not on every draw. */
const drone_img = new Image();
drone_img.onerror = () => console.error(`Cannot load ${DRONE_IMAGE}`);
drone_img.src = DRONE_IMAGE;

/* || ------- View (world -> screen) ------- */

/**
 * Compute the scale and offsets that fit all zones inside the canvas,
 * centered, keeping the aspect ratio.
 *
 * @param {Array<{x: number, y: number}>} zones - Hubs with coordinates.
 * @param {number} [padding=30] - Empty border in pixels.
 * @returns {void}
 */
function init_view(zones, padding = 30) {
    const xs = zones.map(z => z.x);
    const ys = zones.map(z => z.y);
    const min_x = Math.min(...xs);
    const min_y = Math.min(...ys);
    const w = Math.max(...xs) - min_x || 1;
    const h = Math.max(...ys) - min_y;

    const sx = (canvas.width - 2 * padding) / w;
    const sy = (canvas.height - 2 * padding) / h;
    view.scale = Math.min(sx, sy);
    view.min_x = min_x;
    view.min_y = min_y;
    view.off_x = padding + (canvas.width - 2 * padding - w * view.scale) / 2;
    view.off_y = padding + (canvas.height - 2 * padding - h * view.scale) / 2;
}

/**
 * Convert world coordinates to canvas pixel coordinates.
 * Needs init_view() first.
 *
 * @param {number} x - Horizontal world position.
 * @param {number} y - Vertical world position.
 * @returns {{x: number, y: number}} Pixels, origin top-left.
 */
function world_to_screen(x, y) {
    return {
        x: view.off_x + (x - view.min_x) * view.scale,
        y: view.off_y + (y - view.min_y) * view.scale
    };
}

/* || ------- Data loading ------- */

/**
 * Fetch a URL and parse its body as JSON.
 *
 * @param {string} url - Path relative to the page URL.
 * @returns {Promise<any>} The parsed JSON.
 * @throws {Error} On network failure, non-2xx status or invalid JSON.
 */
async function fetch_json(url) {
    const response = await fetch(url);
    if (!response.ok) {
        throw new Error(`HTTP ${response.status} for ${url}`);
    }
    return await response.json();
}

/**
 * Fetch the list of parsed maps.
 *
 * @returns {Promise<Array<Object>>} Index entries, or [] on failure.
 */
async function get_index_data() {
    try {
        return await fetch_json("parsed_maps/index.json");
    } catch (error) {
        console.error(error);
        return [];
    }
}

/**
 * Group index entries by difficulty, keeping the order of the index.
 *
 * @param {Array<Object>} entries - Index entries.
 * @returns {Map<string, Array<Object>>} Difficulty -> entries.
 */
function group_by_difficulty(entries) {
    const groups = new Map();

    for (const entry of entries) {
        if (!groups.has(entry.difficulty)) {
            groups.set(entry.difficulty, []);
        }
        groups.get(entry.difficulty).push(entry);
    }
    return groups;
}

/**
 * Turn a loaded map file into the structure kept in `state.map`, and fit
 * the view to it.
 *
 * @param {{hubs: Array<Object>, connections: Array<Object>}} map
 * @returns {{hubs: Array<Object>, connections: Array<Object>,
 *            by_name: Map<string, Object>}|null} Null if the map has no hubs.
 */
function build_map(map) {
    if (map.hubs.length === 0) {
        return null;
    }
    init_view(map.hubs);
    return {
        hubs: map.hubs,
        connections: map.connections,
        by_name: new Map(map.hubs.map(hub => [hub.name, hub])),
    };
}

/**
 * Path of the simulation file of an index entry. Uses `entry.simulation`
 * if the index provides it, otherwise "<map>.json" -> "<map>_sim.json".
 *
 * @param {Object} entry - Index entry.
 * @returns {string} Path relative to the page URL.
 */
function sim_path_for(entry) {
    return entry.simulation ?? entry.path.replace(/\.json$/, "_sim.json");
}

/**
 * Fetch the simulation file of a map.
 *
 * @param {Object} entry - Index entry.
 * @returns {Promise<Object|null>} The simulation data, or null on failure.
 */
async function load_simulation(entry) {
    try {
        const data = await fetch_json(sim_path_for(entry));

        if (!Array.isArray(data.turns)) {
            throw new Error("Simulation file has no turns list");
        }
        return data;
    } catch (error) {
        console.error(error);
        return null;
    }
}

/* || ------- Simulation logic (no drawing) ------- */

/**
 * Point halfway between two points.
 *
 * @param {{x: number, y: number}} a
 * @param {{x: number, y: number}} b
 * @returns {{x: number, y: number}}
 */
function midpoint(a, b) {
    return { x: (a.x + b.x) / 2, y: (a.y + b.y) / 2 };
}

/**
 * Linear interpolation between two points.
 *
 * @param {{x: number, y: number}} a - Point at t = 0.
 * @param {{x: number, y: number}} b - Point at t = 1.
 * @param {number} t - Progress, 0 to 1.
 * @returns {{x: number, y: number}}
 */
function lerp_point(a, b, t) {
    return { x: a.x + (b.x - a.x) * t, y: a.y + (b.y - a.y) * t };
}

/**
 * Convert the turns of a simulation file into movement segments per drone.
 *
 * A normal move is one segment of one turn. A move that enters a
 * multi-turn connection ("in_transit") goes to the middle of the
 * connection, and the arrival record continues from that middle point.
 * Times are in turn units: turn index i covers t in [i, i + 1).
 *
 * @param {{turns: Array<Object>}} data - Simulation file.
 * @param {{hubs: Array<Object>, by_name: Map<string, Object>}} map
 * @returns {{start: Object, segments: Map<number, Array<Object>>}|null}
 *          Null if the map has no start hub.
 */
function build_timeline(data, map) {
    const start = map.hubs.find(hub => hub.role === "start");
    if (!start) {
        console.error("Map has no start hub");
        return null;
    }

    const segments = new Map();    // drone id -> [{t0, t1, from, to}, ...]
    const midpoints = new Map();   // drone id -> middle point of its transit

    data.turns.forEach((turn, i) => {
        for (const move of turn.turn_data) {
            const a = map.by_name.get(move.from);
            const b = map.by_name.get(move.to);

            if (!a || !b) {
                console.warn("Unknown hub in move:", move);
                continue;
            }
            let from = a;
            let to = b;
            if (move.in_transit) {
                to = midpoint(a, b);
                midpoints.set(move.drone, to);
            } else if (midpoints.has(move.drone)) {
                from = midpoints.get(move.drone);
                midpoints.delete(move.drone);
            }
            if (!segments.has(move.drone)) {
                segments.set(move.drone, []);
            }
            segments.get(move.drone).push({ t0: i, t1: i + 1, from, to });
        }
    });
    return { start, segments };
}

/**
 * Position of a drone at a given simulation time.
 *
 * Before its first move a drone waits at the start hub. Between moves it
 * stays where its last move ended. After its last move it stays there, or
 * disappears if HIDE_ON_ARRIVAL is set.
 *
 * @param {number} id - Drone id.
 * @param {number} t - Simulation time in turns (elapsed / TURN_MS).
 * @returns {{x: number, y: number}|null} World position, or null if hidden.
 */
function drone_position(id, t) {
    const { start, segments } = state.sim.timeline;
    const segs = segments.get(id);

    if (!segs) {
        return start;
    }
    let held = start;
    for (const seg of segs) {
        if (t < seg.t0) {
            return held;
        }
        if (t < seg.t1) {
            return lerp_point(seg.from, seg.to,
                              (t - seg.t0) / (seg.t1 - seg.t0));
        }
        held = seg.to;
    }
    return HIDE_ON_ARRIVAL ? null : held;
}

/**
 * Advance the simulation clock by the time since the previous frame.
 * Time only advances while the simulation is running.
 *
 * @param {number} now - Timestamp given by requestAnimationFrame.
 * @returns {void}
 */
function update_simulation(now) {
    const sim = state.sim;

    if (sim.status === "running") {
        const delta = Math.min(now - (sim.last_frame ?? now), MAX_DELTA_MS);

        sim.elapsed += delta;
        if (sim.elapsed >= sim.total_turns * TURN_MS) {
            sim.elapsed = sim.total_turns * TURN_MS;
            sim.status = "finished";
            update_controls();
        }
    }
    sim.last_frame = now;
}

/* || ------- Drawing ------- */

/**
 * Draw a line between the two hubs of a connection.
 *
 * @param {Map<string, Object>} by_name - Hubs by name.
 * @param {{from: string, to: string}} conn - Connection to draw.
 * @returns {void}
 */
function draw_connection(by_name, conn) {
    const a = by_name.get(conn.from);
    const b = by_name.get(conn.to);

    if (!a || !b) {
        console.warn("Unknown hub in connection:", conn);
        return;
    }
    const p1 = world_to_screen(a.x, a.y);
    const p2 = world_to_screen(b.x, b.y);

    ctx.save();
    ctx.strokeStyle = "grey";
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.stroke();
    ctx.restore();
}

/**
 * Draw a hub as a filled circle. "rainbow" hubs use the current hue.
 *
 * @param {Object} hub - Hub to draw.
 * @param {number} hue - Current rainbow hue, 0 to 359.
 * @returns {void}
 */
function draw_hub(hub, hue) {
    const p = world_to_screen(hub.x, hub.y);
    const color = hub.color === "rainbow"
        ? `hsl(${hue}, 100%, 50%)`
        : hub.color;

    ctx.save();
    ctx.beginPath();
    ctx.arc(p.x, p.y, HUB_RADIUS, 0, 2 * Math.PI);
    ctx.fillStyle = "white";    // fallback: an invalid color is ignored below
    ctx.fillStyle = color ?? "white";
    ctx.strokeStyle = "black";
    ctx.lineWidth = 4;
    ctx.fill();
    ctx.stroke();
    ctx.restore();
}

/**
 * Draw a drone centered on a world position. Uses the drone image, or a
 * plain circle while the image is not loaded (or failed to load).
 *
 * @param {{x: number, y: number}} pos - World position.
 * @returns {void}
 */
function draw_drone(pos) {
    const p = world_to_screen(pos.x, pos.y);

    if (!drone_img.complete || drone_img.naturalWidth === 0) {
        ctx.save();
        ctx.beginPath();
        ctx.arc(p.x, p.y, DRONE_RADIUS, 0, 2 * Math.PI);
        ctx.fillStyle = "#1f3a93";
        ctx.strokeStyle = "white";
        ctx.lineWidth = 2;
        ctx.fill();
        ctx.stroke();
        ctx.restore();
        return;
    }
    const draw_h = DRONE_SIZE * drone_img.naturalHeight
                   / drone_img.naturalWidth;

    // No ctx state is changed here, so no save()/restore() is needed.
    ctx.drawImage(drone_img,
                  p.x - DRONE_SIZE / 2, p.y - draw_h / 2,
                  DRONE_SIZE, draw_h);
}

/**
 * Draw all visible drones at the current simulation time.
 *
 * @returns {void}
 */
function draw_drones() {
    const sim = state.sim;

    if (!sim.timeline) {
        return;
    }
    const t = sim.elapsed / TURN_MS;

    for (let id = 1; id <= sim.drone_count; id++) {
        const pos = drone_position(id, t);

        if (pos) {
            draw_drone(pos);
        }
    }
}

/**
 * Draw the stats panel (current turn, total drones) in the top-left corner.
 * Drawn in screen pixels, so it ignores the map view. Hidden while no
 * simulation is loaded.
 *
 * Turn shown: 0 before Start, then 1 to total_turns (stays on the last
 * turn when finished). Derived from the clock, so Pause freezes it.
 *
 * @returns {void}
 */
function draw_stats() {
    const sim = state.sim;

    if (!sim.timeline) {
        return;
    }
    const turn = sim.status === "idle"
        ? 0
        : Math.min(Math.floor(sim.elapsed / TURN_MS) + 1, sim.total_turns);
    const lines = [
        `Turn ${turn}/${sim.total_turns}`,
        `Drones ${sim.drone_count}`,
    ];

    ctx.save();
    ctx.font = `${HUD_FONT_SIZE}px Main_Title, monospace`;
    ctx.textAlign = "left";
    ctx.textBaseline = "top";

    const line_h = HUD_FONT_SIZE * 1.5;
    const text_w = Math.max(...lines.map(line => ctx.measureText(line).width));
    const text_h = (lines.length - 1) * line_h + HUD_FONT_SIZE;
    const panel_w = text_w + 2 * HUD_PAD;
    const panel_h = text_h + 2 * HUD_PAD;

    ctx.fillStyle = "rgba(255, 255, 255, 0.85)";
    ctx.strokeStyle = "grey";
    ctx.lineWidth = 3;
    ctx.fillRect(HUD_MARGIN, HUD_MARGIN, panel_w, panel_h);
    ctx.strokeRect(HUD_MARGIN, HUD_MARGIN, panel_w, panel_h);

    ctx.fillStyle = "#222";
    lines.forEach((line, i) => {
        ctx.fillText(line,
                     HUD_MARGIN + HUD_PAD,
                     HUD_MARGIN + HUD_PAD + i * line_h);
    });
    ctx.restore();
}

/**
 * Draw one whole frame: connections, hubs, drones, then the stats on top.
 *
 * @param {number} now - Timestamp given by requestAnimationFrame.
 * @returns {void}
 */
function render(now) {
    const map = state.map;
    const hue = (Math.floor(now / RAINBOW_STEP_MS) * 10) % 360;

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    if (!map) {
        return;
    }
    for (const conn of map.connections) {
        draw_connection(map.by_name, conn);
    }
    for (const hub of map.hubs) {
        draw_hub(hub, hue);
    }
    draw_drones();
    draw_stats();
}

/**
 * The only animation loop: update the clock, draw, schedule the next frame.
 *
 * @param {number} now - Timestamp given by requestAnimationFrame.
 * @returns {void}
 */
function frame(now) {
    update_simulation(now);
    render(now);
    requestAnimationFrame(frame);
}

/* || ------- Simulation controls ------- */

/**
 * Enable or disable the Start / Pause / Restart buttons from the state.
 *
 * @returns {void}
 */
function update_controls() {
    const sim = state.sim;
    const ready = sim.timeline !== null;

    start_btn.disabled = !ready || sim.status === "running";
    pause_btn.disabled = sim.status !== "running";
    restart_btn.disabled = !ready || sim.status === "idle" 
                           || sim.status == "finished";
}

/**
 * Start, resume, or (after the end) replay the simulation.
 *
 * @returns {void}
 */
function start_simulation() {
    const sim = state.sim;

    if (!sim.timeline || sim.status === "running") {
        return;
    }
    if (sim.status === "finished") {
        sim.elapsed = 0;
    }
    sim.status = "running";
    update_controls();
}

/**
 * Pause the simulation where it is.
 *
 * @returns {void}
 */
function pause_simulation() {
    if (state.sim.status === "running") {
        state.sim.status = "paused";
        update_controls();
    }
}

/**
 * Go back to turn 0 and run again.
 *
 * @returns {void}
 */
function restart_simulation() {
    const sim = state.sim;

    if (!sim.timeline) {
        return;
    }
    sim.elapsed = 0;
    sim.status = "running";
    update_controls();
}

/* || ------- UI ------- */

/**
 * Return the entries of the selected difficulty.
 *
 * @returns {Array<Object>} Entries, or [] if nothing is selected.
 */
function current_list() {
    return state.groups.get(state.difficulty) ?? [];
}

/**
 * Sync the map name and the prev/next buttons with the state.
 *
 * @returns {void}
 */
function update_nav() {
    const list = current_list();

    map_name.textContent = list[state.position]?.name ?? "-";
    prev_btn.disabled = state.position <= 0;
    next_btn.disabled = state.position >= list.length - 1;
}

/**
 * Fill the difficulty dropdown from the grouped data.
 *
 * @returns {void}
 */
function fill_difficulties() {
    difficulty_select.replaceChildren();

    for (const difficulty of state.groups.keys()) {
        const option = document.createElement("option");
        option.value = difficulty;
        option.textContent = difficulty;
        difficulty_select.appendChild(option);
    }
    difficulty_select.disabled = state.groups.size === 0;
}

/**
 * Load the selected map and its simulation into `state`. Stops the running
 * simulation first. Ignores the results if a newer request was started in
 * the meantime. Drawing is done by the frame loop.
 *
 * @returns {Promise<void>}
 */
async function show_map() {
    const entry = current_list()[state.position];
    const id = ++state.request;

    update_nav();
    state.map = null;
    state.sim = new_sim();
    update_controls();
    if (!entry) {
        return;
    }
    try {
        const map = build_map(await fetch_json(entry.path));

        if (id !== state.request) {
            return;
        }
        state.map = map;
        if (!map) {
            return;
        }

        const data = await load_simulation(entry);

        if (id !== state.request || !data) {
            return;
        }
        const timeline = build_timeline(data, map);

        if (timeline) {
            state.sim = {
                ...new_sim(),
                timeline,
                drone_count: data.drones,
                total_turns: data.turns.length,
            };
            update_controls();
        }
    } catch (error) {
        console.error(error);
    }
}

/**
 * Move the selection inside the current difficulty (clamped).
 *
 * @param {number} delta - +1 for next, -1 for previous.
 * @returns {void}
 */
function step(delta) {
    const last = current_list().length - 1;
    const target = Math.min(Math.max(state.position + delta, 0), last);

    if (target === state.position) {
        return;
    }
    state.position = target;
    show_map();
}

/**
 * Handle a difficulty change: select its first map.
 *
 * @returns {void}
 */
function on_difficulty_change() {
    state.difficulty = difficulty_select.value;
    state.position = 0;
    show_map();
}

/* || ------- Entry point ------- */

/**
 * Initial set-up: attach listeners, start the frame loop, load the index
 * and show the first map.
 *
 * @returns {Promise<void>}
 */
async function main() {
    difficulty_select.addEventListener("change", on_difficulty_change);
    prev_btn.addEventListener("click", () => step(-1));
    next_btn.addEventListener("click", () => step(1));
    start_btn.addEventListener("click", start_simulation);
    pause_btn.addEventListener("click", pause_simulation);
    restart_btn.addEventListener("click", restart_simulation);
    update_controls();
    requestAnimationFrame(frame);

    const entries = await get_index_data();

    state.groups = group_by_difficulty(entries);
    fill_difficulties();
    if (state.groups.size === 0) {
        map_name.textContent = "No maps";
        prev_btn.disabled = true;
        next_btn.disabled = true;
        return;
    }
    state.difficulty = difficulty_select.value;
    state.position = 0;
    await show_map();
}

main();