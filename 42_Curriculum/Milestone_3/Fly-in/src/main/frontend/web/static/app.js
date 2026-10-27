const view = { min_x: 0, min_y: 0, scale: 1, off_x: 0, off_y: 0 };

// /**
//  * Draws a hub on the canvas.
//  *
//  * @param {number} x - Horizontal position in screen pixels.
//  * @param {number} y - Vertical position in screen pixels.
//  * @param {string} color - Any CSS color string.
//  * @returns {void}
//  */
// function draw_hub(x, y, color) {

// }

// function draw_connection(start_x, start_y, end_x, end_y) {

// }
function init_view(zones, canvas, padding = 40)
{
	const xs = zones.map(z => z.x);
	const ys = zones.map(z => z.y);
	const min_x = Math.min(...xs);
	const min_y = Math.min(...ys);
	const w = Math.max(...xs) - min_x || 1;
	const h = Math.max(...ys) - min_y || 1;

	const sx = (canvas.width - 2 * padding) / w;
	const sy = (canvas.height - 2 * padding) / h;
	view.scale = Math.min(sx, sy);
	view.min_x = min_x;
	view.min_y = min_y;
	view.off_x = padding + (canvas.width - 2 * padding - w * view.scale) / 2;
	view.off_y = padding + (canvas.height - 2 * padding - h * view.scale) / 2;
}

/**
 * Converts world coordinates to canvas pixel coordinates.
 * 
 * @param {number} x - Horizontal position in screen pixels.
 * @param {number} y - Vertical position in screen pixels.
 * @returns {x, y} (floats, origin top-left). Needs init_view() first.
 */
function world_to_screen(x, y)
{
	return {
		x: view.off_x + (x - view.min_x) * view.scale,
		y: view.off_y + (y - view.min_y) * view.scale
	};
}

/**
 * Fetch the list of parsed maps.
 *
 * @returns {Promise<Array<Object>>} Index entries, or [] on failure.
 */
async function get_index_data() {
    try {
        const response = await fetch('parsed_maps/index.json');
        if (!response.ok) {
            throw new Error(`HTTP ${response.status}`);
        }
        return await response.json();
    } catch (error) {
        console.error(error);
        return [];
    }
}

function draw_line() {
    /** @type {HTMLCanvasElement} */
    const canvas = document.getElementById("simulation");
    const ctx = canvas.getContext("2d")

    ctx.moveTo(0, 0);
    ctx.lineTo(200, 200);
    ctx.stroke();
}

function draw_circle() {
    /** @type {HTMLCanvasElement} */
    const canvas = document.getElementById("simulation");
    const ctx = canvas.getContext("2d");

    ctx.beginPath();
    ctx.arc(95, 50, 40, 0, 2 * Math.PI);
    ctx.stroke();
}

function reset_canvas() {
    /** @type {HTMLCanvasElement} */
    const canvas = document.getElementById("simulation");
    const ctx = canvas.getContext('2d');

    ctx.clearRect(0, 0, canvas.width, canvas.height);
}

function draw_all() {
    draw_line();
    draw_circle();
}

/**
 * 
 */
function display_map_name(index) {
    const map_name = document.getElementById("mapName");

    map_name.textContent = index[0]["name"]
}

function map_left() {
    count = 0
}
/**
 * @brief Sets the (0, 0) cords of the canvas to the middle of the canvas
 *        instead of the top-left corner.
 */
function set_canvas_cords() {
    /** @type {HTMLCanvasElement} */
    const canvas = document.getElementById("simulation");
    const ctx = canvas.getContext('2d');

    ctx.translate(canvas.width / 2, canvas.height / 2);
}

async function main() {
    const index = await get_index_data();

    display_map_name(index);


}

main();