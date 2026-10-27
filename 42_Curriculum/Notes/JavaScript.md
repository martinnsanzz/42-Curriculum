# JavaScript

This *markdown file* will be used to write down everything related to `JavaScript`, such as
syntax, data types, functions, the DOM, asynchronous code and other things related to the
language itself.

## JavaScript Basics
`JavaScript` (JS) is the programming language of the web. HTML gives the structure, CSS the
appearance, and JS the **behavior**: reacting to clicks, changing the page, loading data,
drawing on a canvas.

Key facts (compared to C):

- **Interpreted:** no compilation step, the browser runs the file directly.
- **Dynamically typed:** variables have no declared type, the **value** has the type.
- **Garbage collected:** no `malloc`/`free`, no pointers, no manual memory.
- **Single-threaded:** one thing runs at a time, long waits are handled with `async` code.
- **Everything is an object** (almost): functions and arrays are objects too.

>**Notes:** JavaScript has **nothing to do with Java**. The name is historical.

### Adding JS to HTML
```html
<!-- Internal: code inside the page -->
<script>
    console.log("Hello");
</script>

<!-- External: separate file (best option) -->
<script src="app.js" defer></script>

<!-- ES module: allows import / export -->
<script type="module" src="app.js"></script>
```

- **No attribute:** the browser stops reading the HTML, runs the script, then continues.
  If the script is in `<head>`, the elements **do not exist yet** (`getElementById` returns
  `null`).
- **`defer`:** downloads while the HTML is parsed, runs **after** the HTML is fully parsed,
  in order. Use this by default.
- **`type="module"`:** deferred by default, has its own scope and allows `import`/`export`.
  Must be served over HTTP (not `file://`).

### Syntax basics
```js
// single line comment
/* multi line
   comment */

let name = "Martin";   // semicolons are optional, but use them for consistency
console.log(name);     // prints to the browser console (F12 -> Console)
```

- Case sensitive: `name` and `Name` are different.
- Statements end with `;` (automatic semicolon insertion exists, but can cause bugs).
- Naming: `camelCase` for variables/functions, `PascalCase` for classes,
  `UPPER_SNAKE_CASE` for constants.

>**Notes:** Use `"use strict";` at the top of a file (or modules, which are strict by
>default) to turn silent mistakes, like using an undeclared variable, into real errors.

## Variables

|Keyword|Reassign|Scope|Notes|
|---|---|---|---|
|`const`|no|block|**default choice**|
|`let`|yes|block|when the value changes|
|`var`|yes|function|old style, **avoid**|

```js
const PADDING = 40;     // cannot be reassigned
let count = 0;
count = count + 1;      // ok
PADDING = 10;           // TypeError
```

>**Notes:** `const` protects the **variable**, not the **content**. A `const` array or
>object can still be modified (`arr.push(1)` works). Only `arr = []` fails.

### Scope
- **Block scope** (`let`/`const`): lives inside the nearest `{ }`.
- **Function scope** (`var`): ignores blocks like `if` and `for`, a classic source of bugs.
- **Global scope:** declared outside everything. Accessible everywhere, so keep it minimal.

```js
if (true) {
    let a = 1;
    var b = 2;
}
console.log(b);   // 2
console.log(a);   // ReferenceError
```

### Hoisting
Declarations are moved to the top of their scope **before** execution.

- `var` is hoisted and initialized as `undefined`.
- `let`/`const` are hoisted but **not usable** before their line (Temporal Dead Zone,
  throws `ReferenceError`).
- Function declarations are fully hoisted: you can call them before they are written.

## Data Types
There are **7 primitive types** and **objects**.

|Type|Example|Notes|
|---|---|---|
|`number`|`42`, `3.14`, `-1`|one type for ints and floats (64-bit double)|
|`string`|`"hi"`, `'hi'`, `` `hi` ``|immutable|
|`boolean`|`true`, `false`||
|`undefined`|`undefined`|variable declared but no value assigned|
|`null`|`null`|"intentionally empty"|
|`bigint`|`123n`|integers of any size|
|`symbol`|`Symbol("id")`|unique identifier (rarely needed)|
|`object`|`{}`, `[]`, functions|everything else|

```js
typeof 42;          // "number"
typeof "hi";        // "string"
typeof undefined;   // "undefined"
typeof null;        // "object"  <- historical bug of the language
typeof [];          // "object"  <- use Array.isArray(x) instead
typeof function(){};// "function"
```

### Numbers
- All numbers are **floating point**: `0.1 + 0.2` gives `0.30000000000000004`.
  Never compare floats with `===`, compare with a tolerance.
- Safe integer range: `Number.MAX_SAFE_INTEGER` (2^53 - 1). Beyond it use `bigint`.
- Special values: `NaN` (Not a Number), `Infinity`, `-Infinity`.
- `NaN !== NaN`. Check with `Number.isNaN(x)`.
- Integer division does **not** exist: `7 / 2` is `3.5`. Use `Math.floor(7 / 2)`.

```js
Math.min(...[3, 1, 2]);       // 1 (spread the array into arguments)
Math.max();                    // -Infinity (empty call, edge case!)
Math.round(2.5);               // 3
Math.floor(-2.5);              // -3  (floor goes down, not toward zero)
Math.trunc(-2.5);              // -2  (toward zero, like C int cast)
parseInt("42px");              // 42
Number("42px");                // NaN (stricter than parseInt)
(3.14159).toFixed(2);          // "3.14" (returns a STRING)
```

### Strings
```js
const s = "Hello";
s.length;                  // 5
s[0];                      // "H"
s.toUpperCase();           // "HELLO"
s.includes("ell");         // true
s.indexOf("l");            // 2  (-1 if not found)
s.slice(1, 3);             // "el"
s.split("");               // ["H","e","l","l","o"]
s.trim();                  // removes spaces at both ends
s.replace("l", "L");       // "HeLlo" (only the first match)
s.replaceAll("l", "L");    // "HeLLo"
s.padStart(8, "0");        // "000Hello"
```

- Strings are **immutable**: methods return a **new** string, the original never changes.
- **Template literals** use backticks, allow variables and multiple lines:

```js
const hubs = 3;
console.log(`There are ${hubs} hubs, ${hubs * 2} total.`);
```

### Truthy and falsy
In conditions every value is converted to a boolean.

**Falsy (only these 8):** `false`, `0`, `-0`, `0n`, `""`, `null`, `undefined`, `NaN`.
**Everything else is truthy**, including `"0"`, `"false"`, `[]` and `{}`.

```js
if ([]) { /* runs! an empty array is truthy */ }
if ([].length) { /* correct way to check for empty */ }
```

## Operators

|Operator|Description|
|---|---|
|`+ - * / % **`|arithmetic (`**` is power)|
|`++ --`|increment / decrement|
|`== !=`|loose equality (converts types) **avoid**|
|`=== !==`|strict equality (no conversion) **use this**|
|`&& \|\| !`|logical|
|`??`|nullish coalescing|
|`?.`|optional chaining|
|`? :`|ternary|

```js
1 == "1";        // true   (converted)
1 === "1";       // false  (different types)
null == undefined;   // true
null === undefined;  // false
```

>**Notes:** Always use `===` and `!==`. Loose equality has confusing rules
>(`"" == 0` is `true`).

### `||` vs `??`
- `a || b`: uses `b` if `a` is **falsy** (also for `0` and `""`).
- `a ?? b`: uses `b` only if `a` is `null` or `undefined`.

```js
const count = 0;
count || 10;   // 10  (wrong if 0 is a valid value)
count ?? 10;   // 0   (correct)
```

### Optional chaining
Stops and returns `undefined` instead of throwing if something in the chain is
`null`/`undefined`.

```js
hub.metadata?.color;        // undefined if metadata is missing
hub.metadata?.color ?? "white";   // with a default
```

### Plus operator trap
`+` with a string concatenates, all other math operators convert to numbers.

```js
"5" + 3;    // "53"
"5" - 3;    // 2
```

## Control Flow
```js
if (x > 0) {
    ...
} else if (x === 0) {
    ...
} else {
    ...
}

const label = x > 0 ? "positive" : "not positive";   // ternary

switch (role) {
    case "start":
        ...
        break;               // without break it falls through (like C)
    default:
        ...
}
```

### Loops
```js
for (let i = 0; i < 5; i++) { ... }          // classic

for (const item of array) { ... }            // values of an iterable (array, string, Map)
for (const key in object) { ... }            // KEYS of an object (not for arrays!)

while (cond) { ... }
do { ... } while (cond);

break;       // exit the loop
continue;    // next iteration
```

- Use `for...of` for arrays. `for...in` on arrays gives indexes as **strings** and can include
  inherited properties.
- `for...of` with `.entries()` gives index and value:

```js
for (const [i, value] of array.entries()) { ... }
```

## Functions
```js
// 1. Declaration (hoisted)
function add(a, b) {
    return a + b;
}

// 2. Expression (not hoisted)
const add = function (a, b) { return a + b; };

// 3. Arrow function
const add = (a, b) => a + b;          // implicit return (one expression)
const square = x => x * x;            // one parameter: no parentheses needed
const log = () => { console.log(1); } // braces: you need an explicit return
```

- A function without `return` returns `undefined`.
- Functions are **values**: you can store them, pass them as arguments and return them.
- No overloading: a function is called with **any** number of arguments, missing ones
  are `undefined`, extra ones are ignored.

### Default, rest and spread
```js
function init(zones, padding = 40) { ... }     // default value

function sum(...nums) {                         // rest: collects arguments in an array
    return nums.reduce((a, b) => a + b, 0);
}

Math.max(...[1, 2, 3]);                         // spread: expands an array
```

### Arguments are passed by value, but objects are references
- Primitives are **copied** into the function.
- Objects and arrays: the **reference** is copied, so the function can modify the
  original content.

```js
function reset(arr) { arr.length = 0; }    // modifies the caller's array
function fake(arr)  { arr = []; }          // only changes the local variable
```

>**Notes:** Think of an object variable as a **pointer** that is dereferenced automatically.
>`const a = {}; const b = a;` makes both names point to the same object.

### Closures
A function **remembers the variables of the scope where it was created**, even after that
scope has finished.

```js
function makeCounter() {
    let count = 0;
    return () => ++count;
}
const next = makeCounter();
next();   // 1
next();   // 2   (count lives on, private)
```

Common use: private state, callbacks that need data, event handlers.

### `this`
The value of `this` depends on **how a function is called**, not where it is written.

- Regular function: `this` is whatever is **before the dot** at the call (`obj.method()`).
- **Arrow function:** has **no own `this`**, it uses the one from the surrounding code.
- Detached methods lose `this`: `const f = obj.method; f();` -> `this` is `undefined`.

>**Notes:** In callbacks inside classes use arrow functions, or `bind`, to keep `this`.

### Callbacks and higher-order functions
A **callback** is a function passed to another function to be called later.

```js
[1, 2, 3].forEach(n => console.log(n));
button.addEventListener("click", () => { ... });
setTimeout(() => { ... }, 1000);      // run once after 1000 ms
setInterval(() => { ... }, 1000);     // run every 1000 ms (clearInterval to stop)
```

## Objects
Collections of `key: value` pairs.

```js
const hub = {
    name: "start",
    x: 3,
    y: 5,
    metadata: { color: "red" },
    describe() { return `${this.name} at ${this.x},${this.y}`; }
};

hub.name;               // "start"  (dot notation)
hub["name"];            // "start"  (bracket: key can be a variable)
hub.zone = 2;           // add a property
delete hub.zone;        // remove it
"name" in hub;          // true
```

```js
Object.keys(hub);        // ["name", "x", "y", ...]
Object.values(hub);
Object.entries(hub);     // [["name","start"], ...]
Object.assign({}, hub);  // shallow copy
```

### Shorthand, destructuring and spread
```js
const x = 1, y = 2;
const p = { x, y };                    // same as { x: x, y: y }

const { name, metadata: { color } } = hub;   // destructuring
const { x: px = 0 } = hub;                    // rename + default value

const copy = { ...hub, name: "end" };         // spread: shallow copy with changes
```

### Copying: shallow vs deep
- `{ ...obj }`, `[...arr]`, `Object.assign` copy only the **first level**. Nested objects
  are still shared.
- Deep copy: `structuredClone(obj)`.

```js
const a = { inner: { v: 1 } };
const b = { ...a };
b.inner.v = 99;
a.inner.v;                  // 99 (shared!)
```

### `Map` and `Set`
- `Map`: key-value with **any** type of key, keeps insertion order, fast lookup.
- `Set`: unique values.

```js
const hubs = new Map();
hubs.set("start", hub);
hubs.get("start");
hubs.has("start");
hubs.size;
for (const [name, h] of hubs) { ... }

const seen = new Set([1, 2, 2, 3]);   // {1, 2, 3}
seen.add(4); seen.has(2);
```

>**Notes:** A `Map` built from a list (`new Map(list.map(h => [h.name, h]))`) is the standard
>way to look elements up by name in O(1) instead of searching the array each time.

## Arrays
Ordered, resizable lists. Can hold any mix of types.

```js
const arr = [10, 20, 30];
arr.length;          // 3
arr[0];              // 10
arr[10];             // undefined (no bounds error, unlike C)
arr.at(-1);          // 30 (last element)
```

### Modifying (change the original)
|Method|Description|
|---|---|
|`push(x)` / `pop()`|add / remove at the **end**|
|`unshift(x)` / `shift()`|add / remove at the **start**|
|`splice(i, n, ...items)`|remove `n` items at `i` and insert items|
|`sort(fn)`|sort in place|
|`reverse()`|reverse in place|

### Transforming (return something new)
|Method|Description|
|---|---|
|`map(fn)`|new array with `fn` applied to every element|
|`filter(fn)`|new array with the elements where `fn` is true|
|`reduce(fn, init)`|combine everything into a **single value**|
|`find(fn)`|first element where `fn` is true (or `undefined`)|
|`findIndex(fn)`|index of it (or `-1`)|
|`some(fn)` / `every(fn)`|is **any** / **all** true|
|`includes(x)`|contains the value|
|`slice(a, b)`|copy of a part|
|`concat(...)`, `[...a, ...b]`|join arrays|
|`join(sep)`|array to string|
|`flat()`|flatten one level|

```js
const xs = zones.map(z => z.x);                 // [x1, x2, ...]
const big = zones.filter(z => z.x > 10);
const total = xs.reduce((acc, x) => acc + x, 0);
const start = zones.find(z => z.role === "start");

// group by a key
const byDiff = entries.reduce((acc, e) => {
    (acc[e.difficulty] ??= []).push(e);
    return acc;
}, {});
```

### Pitfalls
- `sort()` without a function sorts **as strings**: `[10, 2, 1].sort()` gives `[1, 10, 2]`.
  Use `arr.sort((a, b) => a - b)`.
- `Math.min(...arr)` / `Math.max(...arr)` with a huge array (~100k+ elements) can overflow
  the call stack. Use `reduce` for very large arrays.
- `arr.forEach` ignores `await` inside and cannot `break`. Use `for...of` for those cases.
- Arrays are objects: `[1] === [1]` is `false` (compares references).

## Classes
Syntax on top of JS's prototype system. Good for grouping state and behavior.

```js
class Drone {
    #id;                         // private field (only accessible inside the class)
    static count = 0;            // belongs to the class, not to instances

    constructor(id, hub) {
        this.#id = id;
        this.hub = hub;
        Drone.count++;
    }

    get id() { return this.#id; }        // getter: used as drone.id

    moveTo(hub) {                         // method
        this.hub = hub;
    }
}

class FastDrone extends Drone {
    constructor(id, hub) {
        super(id, hub);                   // must be called before using this
        this.speed = 2;
    }
}

const d = new Drone(1, "start");          // new is required
```

## Error Handling
```js
try {
    const data = JSON.parse(text);
} catch (err) {
    console.error("Invalid JSON:", err.message);
} finally {
    // always runs
}

throw new Error("Something went wrong");   // throw Error objects, not strings
```

- JS does **not** crash on most mistakes: it silently gives `undefined` or `NaN`. Check values.
- Common errors: `TypeError` (e.g. reading a property of `undefined`),
  `ReferenceError` (variable does not exist), `SyntaxError`.
- Read the error in the console: it tells the file and line.

## JSON
Text format to exchange data. Looks like a JS object, but is stricter.

```js
const text = JSON.stringify(obj);          // object -> string
const text = JSON.stringify(obj, null, 4); // pretty with 4 spaces
const obj = JSON.parse(text);              // string -> object (throws if invalid)
```

- Keys must be in `"double quotes"`. No comments, no trailing commas.
- Types allowed: object, array, string, number, boolean, `null`.
- `undefined`, functions and `Map`/`Set` are **lost or changed** when stringified.

## Asynchronous JavaScript
JS runs one thing at a time. Slow operations (network, timers) are started and JS goes on,
then the **event loop** runs your code again when the result is ready.

```js
console.log(1);
setTimeout(() => console.log(2), 0);
console.log(3);
// prints 1, 3, 2   (the callback waits until the current code finishes)
```

### Promises
An object that represents a **future value**. States: `pending` -> `fulfilled` or `rejected`.

```js
fetch(url)
    .then(response => response.json())
    .then(data => { ... })
    .catch(err => console.error(err));
```

### `async` / `await`
Cleaner syntax on top of promises. Code reads top to bottom.

```js
async function loadIndex() {
    try {
        const response = await fetch("parsed_maps/index.json");
        if (!response.ok) {
            throw new Error(`HTTP ${response.status}`);
        }
        return await response.json();
    } catch (err) {
        console.error("Load failed:", err);
        return [];
    }
}

const index = await loadIndex();      // await outside a function only works in modules
```

- An `async` function **always returns a Promise**.
- `await` pauses **only that function**, not the whole program.
- You can only use `await` inside an `async` function (or at the top level of a module).
- `fetch` only rejects on **network errors**. A 404 is a normal response: check `response.ok`.
- **Sequential vs parallel:**

```js
const a = await load("a");          // waits, then...
const b = await load("b");          // ...starts this one

const [a, b] = await Promise.all([load("a"), load("b")]);   // both at the same time
```

>**Notes:** The most common bug: using a value **before** the `await` has finished.
>`let data; load().then(d => data = d); console.log(data);` prints `undefined`.

## The DOM
The **Document Object Model** is the page as a tree of objects that JS can read and change.

### Selecting elements
```js
document.getElementById("simulation");      // one element by id (or null)
document.querySelector(".card");            // first match of a CSS selector
document.querySelectorAll("button");        // all matches (NodeList)
```

### Changing elements
```js
el.textContent = "Hello";       // set text (safe)
el.innerHTML = "<b>Hi</b>";     // set HTML (never with user data: XSS risk)
el.classList.add("active");     // also: remove, toggle, contains
el.style.color = "red";         // inline style (prefer classes)
el.setAttribute("disabled", "");
el.dataset.id = "5";            // data-id attribute
```

### Creating elements
```js
const option = document.createElement("option");
option.value = entry.path;
option.textContent = entry.name;
select.appendChild(option);      // or select.append(option)
element.remove();                // delete it
```

### Events
```js
button.addEventListener("click", (event) => {
    console.log(event.target);
});

select.addEventListener("change", () => console.log(select.value));
window.addEventListener("resize", onResize);
document.addEventListener("DOMContentLoaded", main);   // HTML parsed
```

Common events: `click`, `input`, `change`, `submit`, `keydown`, `keyup`, `mousemove`,
`mousedown`, `resize`, `load`.

- `event.preventDefault()` stops the default behavior (e.g. a form reloading the page).
- Pass a **function reference**, not a call: `addEventListener("click", draw)`, not
  `addEventListener("click", draw())`.
- To remove a listener you need the **same function reference**.

### Animation
```js
function frame(timestamp) {
    update();
    draw();
    requestAnimationFrame(frame);    // ask for the next frame (~60 per second)
}
requestAnimationFrame(frame);
```

Prefer `requestAnimationFrame` over `setInterval` for drawing: it is synchronized with the
screen and pauses in background tabs.

## Canvas API
Basic 2D drawing (see the HTML and CSS notes for the element itself).

```js
const canvas = document.getElementById("simulation");
const ctx = canvas.getContext("2d");

ctx.clearRect(0, 0, canvas.width, canvas.height);   // erase everything

ctx.beginPath();                    // ALWAYS start a new path
ctx.moveTo(10, 10);
ctx.lineTo(100, 50);
ctx.strokeStyle = "grey";
ctx.lineWidth = 2;
ctx.stroke();

ctx.beginPath();
ctx.arc(x, y, radius, 0, 2 * Math.PI);    // angles in radians
ctx.fillStyle = "tomato";
ctx.fill();

ctx.fillRect(x, y, w, h);
ctx.fillText("text", x, y);
ctx.font = "16px 'Press Start 2P'";
```

- Origin `(0, 0)` is the **top-left**, `y` grows **downward**.
- The canvas is **immediate mode**: it is only pixels. To "move" something, clear and
  redraw everything.
- Without `beginPath()`, new shapes are added to the previous path and old lines get
  redrawn.
- `ctx.save()` / `ctx.restore()` store and restore styles and transforms.
- `ctx.translate()`, `scale()` and `rotate()` **accumulate**. Reset with
  `ctx.setTransform(1, 0, 0, 1, 0, 0)`.
- Fonts must be loaded before drawing text: `await document.fonts.load("16px Main_Title")`.

## Modules
Split code into files with their own scope.

```js
// math.js
export const PADDING = 40;
export function scale(a, b) { ... }
export default class Camera { ... }       // one default export per file

// app.js
import Camera, { PADDING, scale } from "./math.js";
import * as math from "./math.js";
```

- The path needs `./` and the **extension** (`.js`).
- Top-level variables in a module are **not global**.
- Modules run once, even if imported many times.
- They require `<script type="module">` and an HTTP server.

## Debugging
- `console.log(x)`, `console.table(array)`, `console.error()`, `console.dir(obj)`.
- **DevTools (F12):**
  - **Console:** logs and errors.
  - **Sources:** breakpoints, step through code. `debugger;` pauses the code there.
  - **Network:** see every `fetch`, status code, response and timing.
  - **Elements:** the live DOM and computed CSS.
- Check the **Network tab** first when data does not load (404, wrong path, cached response).
- Disable cache while DevTools is open, or use a hard reload (Ctrl+Shift+R).

## Common Pitfalls

- Using `==` instead of `===`.
- Forgetting `await`, so you get a `Promise` object instead of the value.
- Running code before the DOM exists (script in `<head>` without `defer`).
- Reading a property of `undefined`: `Cannot read properties of undefined`. Use `?.` or
  check first.
- `[] === []` and `{} === {}` are `false`: objects compare by reference.
- Float math: `0.1 + 0.2 !== 0.3`.
- Modifying an array while looping over it.
- `typeof null === "object"`.
- `parseInt("08px")` works, `Number("08px")` is `NaN`: know which one you need.
- Global variables leaking by assigning an undeclared name (strict mode catches it).
- Using `var` inside loops with callbacks: all callbacks share the same variable.
- `.sort()` without a comparator sorts numbers as text.
- Mutating function arguments (objects and arrays) by accident.
- `this` lost in a detached method or callback.