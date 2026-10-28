/* || ------- State ------- */

const view = { min_x: 0, min_y: 0, scale: 1, off_x: 0, off_y: 0 };

const state = {
    groups: new Map(),   // difficulty -> list of index entries
    difficulty: null,    // currently selected difficulty
    position: 0,         // position inside the selected difficulty's list
    request: 0,          // id of the latest map request (detects stale responses)
};

let globalHue = 0;  // hue value for color. Same for all rainbow hubs
let animationFrameId = null;    // Keep track of active loop

/* || ------- Time Variables ------- */
let lastTime = 0;
const fpsWanted = 10;
const fpsInterval = 1000 / fpsWanted // ~33.33ms per frame


/* || ------- DOM references ------- */

/** @type {HTMLCanvasElement} */
const canvas = document.getElementById("simulation");
const ctx = canvas.getContext("2d");
const difficulty_select = document.getElementById("difficultySelect");
const prev_btn = document.getElementById("prevBtn");
const next_btn = document.getElementById("nextBtn");
const map_name = document.getElementById("mapName");

/* || ------- View (world -> screen) ------- */

/**
 * Compute the scale and offsets that fit all zones inside the canvas,
 * centered, keeping the aspect ratio.
 *
 * @param {Array<{x: number, y: number}>} zones - Hubs with coordinates.
 * @param {number} [padding=40] - Empty border in pixels.
 * @returns {void}
 */
function init_view(zones, padding = 30) {
    const xs = zones.map(z => z.x);
    const ys = zones.map(z => z.y);
    const min_x = Math.min(...xs);
    const min_y = Math.min(...ys);
    const w = Math.max(...xs) - min_x;
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

/* || ------- Drawing ------- */

/**
 * Draw a map: connections first, hubs on top.
 *
 * @param {{hubs: Array<Object>, connections: Array<Object>}} map
 * @returns {void}
 */
function draw_map(map) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    if (map.hubs.length === 0) {
        return;
    }
    init_view(map.hubs);

    const by_name = new Map(map.hubs.map(hub => [hub.name, hub]));

    ctx.strokeStyle = "grey";
    ctx.lineWidth = 2;
    for (const conn of map.connections) {
        draw_connection(by_name, conn)
    }

    ctx.strokeStyle = "black";
    for (const hub of map.hubs) {
        if (hub.color === "rainbow")
            draw_rainbow_hub(hub);
        else 
            draw_hub(hub)
    }
}

/**
 * Draws a line from point a to point b on the canvas.
 * 
 * @param {Map<any, Object>} by_name, list of hub names.
 * @param {object} conn
 * @returns {void}
 */
function draw_connection(by_name, conn) {
    const a = by_name.get(conn.from);
    const b = by_name.get(conn.to);

    if (!a || !b) {
        console.warn("Unknown hub in connection:", conn);
        return ;
    }
    const p1 = world_to_screen(a.x, a.y);
    const p2 = world_to_screen(b.x, b.y);
    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.stroke();
}

/**
 * Draws circle in the specified position
 * 
 * @param {object} hub, current hub to draw on screen 
 * @return {void}
 */
function draw_hub(hub) {
    const p = world_to_screen(hub.x, hub.y);

    ctx.beginPath();
    ctx.arc(p.x, p.y, 20, 0, 2 * Math.PI);
    ctx.fillStyle = hub.color ?? "white";
    ctx.lineWidth = 4;
    ctx.strokeStyle = "black";
    ctx.fill();
    ctx.stroke();
}

/**
 * Draws circle in the specified position with a changing color
 * animation
 * 
 * @param {object} hub, current hub to draw on screen 
 * @return {void}
 */
function draw_rainbow_hub(hub) {
    const p = world_to_screen(hub.x, hub.y);

    ctx.beginPath();
    ctx.arc(p.x, p.y, 20, 0, 2 * Math.PI);

    ctx.fillStyle = `hsl(${globalHue}, 100%, 50%)`;
    ctx.lineWidth = 4;
    ctx.strokeStyle = "black";
    ctx.fill();
    ctx.stroke();
}

/**
 * Global animation loop in charge of redrawing the map at a controlled frame rate.
 * 
 * It optimizes performance by only animating if the map contains "rainbow" hubs,
 * and caps the redraw rate based on a predefined FPS interval to save resources.
 *
 * @param {{hubs: Array<Object>, connections: Array<Object>}} map - The map object to render.
 * @returns {void}
 */
function startAnimationLoop(map) {
    if (animationFrameId) {
        cancelAnimationFrame(animationFrameId);
        animationFrameId = null;
    }

    const hasRainbow = map.hubs.some(hub => hub.color === "rainbow")

    if (!hasRainbow) {
        draw_map(map);
        return ;
    }

    lastTime = performance.now()

    function animate(currentTime) {
        animationFrameId = requestAnimationFrame(animate);
        
        const elapsedTime = currentTime - lastTime;

        if (elapsedTime >= fpsInterval) {
            lastTime = currentTime - (elapsedTime % fpsInterval)

            globalHue = (globalHue + 10) % 360; 
            draw_map(map);
            console.log("hello") 
        }
    }
    animationFrameId = requestAnimationFrame(animate);
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
 * Load the selected map and draw it. Ignores the result if a newer
 * request was started in the meantime.
 *
 * @returns {Promise<void>}
 */
async function show_map() {
    const entry = current_list()[state.position];
    const id = ++state.request;

    update_nav();
    if (!entry) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        return;
    }
    try {
        const map = await fetch_json(entry.path);
        if (id !== state.request) {
            return;
        }
        startAnimationLoop(map)
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
 * Initial set-up: load the index, fill the dropdown, show the first map.
 *
 * @returns {Promise<void>}
 */
async function main() {
    difficulty_select.addEventListener("change", on_difficulty_change);
    prev_btn.addEventListener("click", () => step(-1));
    next_btn.addEventListener("click", () => step(1));

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