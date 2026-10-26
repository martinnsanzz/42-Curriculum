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
 * @brief Sets the (0, 0) cords of the canvas to the middle of the canvas
 *        instead of the top-left corner.
 */
function set_canvas_cords() {
    /** @type {HTMLCanvasElement} */
    const canvas = document.getElementById("simulation");
    const ctx = canvas.getContext('2d');

    ctx.translate(canvas.width / 2, canvas.height / 2);
}

/**
 * @brief Main function to run for initial set-up.
 */
function main() {
    set_canvas_cords()
}

main();