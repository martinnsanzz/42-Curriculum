# CSS

This *markdown file* will be used to write down everything related to `CSS`, such as
syntax, properties, layout methods and other things related to styling web pages.

## CSS Basics
`CSS` (Cascading Style Sheets) is the language used to **describe how HTML elements look
and are laid out**. HTML gives the structure, CSS gives the appearance: colors, fonts,
spacing, sizes, positions, and how the page adapts to different window sizes.

>**Notes:** "Cascading" means that when several rules affect the same element, the browser
>has a fixed set of rules to decide which one wins (see **Specificity and Cascade**).

### Syntax
A CSS **rule** is made of a `selector` and a `declaration block`.

```css
selector {
    property: value;
    property: value;
}
```

- **Selector:** which HTML elements the rule applies to.
- **Declaration:** a `property: value;` pair. Always ends with `;`.
- **Declaration block:** everything inside `{ }`.

```css
h1 {
    color: blue;
    font-size: 30px;
}
```

**Comments** are written between `/* */` (there is no `//` in CSS):

```css
/* || ------- General Styles ------- */
```

>**Notes:** The `||` trick in the comment is the "Comment Anchors" style used in the project,
>it helps to jump between sections in the editor.

### Ways of adding CSS
1. **Inline:** `style` attribute on one element. Highest priority, hard to maintain.
2. **Internal:** a `<style>` element inside `<head>`. Applies to one page.
3. **External:** a `.css` file linked with `<link rel="stylesheet" href="style.css">`.
   Best option, one file can style the whole website.

(See the HTML notes for examples of each.)

## Selectors
Selectors choose which elements get styled.

|Selector|Example|Matches|
|---|---|---|
|Element|`p`|every `<p>`|
|Class|`.card`|every element with `class="card"`|
|ID|`#title`|the element with `id="title"` (should be unique)|
|Universal|`*`|every element|
|Group|`h1, h2`|both `h1` and `h2`|
|Descendant|`div p`|any `p` **inside** a `div` (any depth)|
|Child|`div > p`|`p` that is a **direct** child of `div`|
|Attribute|`input[type="text"]`|inputs with that attribute value|

**Pseudo-classes** select an element in a certain *state* (single colon):

```css
.btn:hover   { background-color: #059862; }  /* mouse over it */
.btn:active  { transform: scale(0.95); }     /* while being clicked */
li:first-child { font-weight: bold; }        /* first li of its parent */
```

**Pseudo-elements** select a *part* of an element, or create generated content
(double colon): `::before`, `::after`, `::first-line`, `::placeholder`.

### Specificity and Cascade
When two rules target the same property of the same element, the winner is decided in
this order:

1. `!important` (avoid it, it makes debugging painful).
2. **Specificity:** `inline style` > `#id` > `.class / :pseudo-class / [attribute]` > `element`.
3. **Order:** if specificity is equal, the rule written **last** wins.

>**Notes:** Properties like `color` and `font-family` are **inherited**: a child uses the
>parent's value unless told otherwise. Properties like `border`, `margin` and `padding`
>are not inherited.

## Colors and Backgrounds
Colors can be written as:

```css
color: tomato;                /* color name (about 140 available) */
color: #eef1ff;               /* HEX */
color: rgb(238, 241, 255);    /* RGB */
color: rgba(0, 0, 0, 0.5);    /* RGB + alpha (transparency, 0 to 1) */
color: hsl(231, 100%, 97%);   /* hue, saturation, lightness */
```

|Property|Description|
|---|---|
|`color`|text color|
|`background-color`|background color of the element|
|`background-image`|`url(...)` image or gradient|
|`background-size`|`cover`, `contain` or a size|
|`background-repeat`|`no-repeat`, `repeat-x`...|
|`opacity`|transparency of the **whole** element (and its children), 0 to 1|

## Text and Fonts

|Property|Description|
|---|---|
|`font-family`|font to use, **with fallbacks**|
|`font-size`|text size (`px`, `rem`, `%`...)|
|`font-weight`|`normal`, `bold`, or 100 to 900|
|`text-align`|`left`, `center`, `right`, `justify`|
|`line-height`|height of each text line|
|`letter-spacing`|space between letters|
|`text-shadow`|`x y blur color`, useful for neon/glow effects|
|`text-transform`|`uppercase`, `lowercase`, `capitalize`|

### Fallback chain
```css
font-family: "Press Start 2P", monospace;
```
The browser tries each font in order. The last one should be a **generic family**
(`serif`, `sans-serif`, `monospace`). Without a fallback, a missing font = default font.

### Custom fonts with `@font-face`
```css
@font-face {
    font-family: Main_Title;                    /* name you choose */
    src: url(fonts/PressStart2P-Regular.ttf);   /* path relative to the CSS file */
    font-display: swap;                         /* show fallback text while loading */
}

#title {
    font-family: Main_Title, monospace;
}
```

>**Notes:** Pixel fonts look best at multiples of their design size (Press Start 2P: 8, 16,
>24, 32px). Other sizes blur the edges.

## The Box Model
**Every** element is a rectangle made of 4 layers, from inside to outside:

```
content -> padding -> border -> margin
```

|Layer|Description|
|---|---|
|`content`|the text/image/children. Sized by `width` and `height`|
|`padding`|space **inside** the border, the background shows through it|
|`border`|line around the padding|
|`margin`|space **outside**, always transparent|

```css
.box {
    padding: 10px;                 /* all 4 sides */
    padding: 10px 20px;            /* vertical | horizontal */
    padding: 1px 2px 3px 4px;      /* top | right | bottom | left (clockwise) */
    border: 2px solid grey;        /* width | style | color */
    margin: 0 auto;                /* 0 vertical, auto horizontal (centers a block) */
}
```

### `box-sizing`
- `content-box` (**default**): `width`/`height` set only the **content**. Padding and border
  are **added** on top.
  Example: `width: 100px; padding: 10px; border: 5px` -> real width = **130px**.
- `border-box`: `width`/`height` **include** padding and border. The real size is what you wrote.

The usual reset, written once at the top of the file:

```css
*, *::before, *::after {
    box-sizing: border-box;
}
```

- `*` matches every element, but **not** the pseudo-elements, that is why `::before` and
  `::after` are listed too.

### Margin collapsing
Vertical margins between block elements can **merge**: two neighbors with `margin: 20px`
end up 20px apart, not 40px (the bigger one wins). It does not happen with flex/grid items.

### `margin` vs `padding` vs `gap`
- `padding`: inside, background visible.
- `margin`: outside, transparent, may collapse.
- `gap`: spacing **between children** of a flex/grid container. Cleaner than margins.

>**Notes:** Default `margin` of the `body` is 8px and `h1` has big default top/bottom margins
>(about 0.67em). They are a classic hidden cause of unwanted scrollbars.

## Units

|Unit|Meaning|
|---|---|
|`px`|fixed CSS pixel|
|`%`|percentage of the **parent** (what exactly depends on the property)|
|`em`|relative to the **current element's** font size|
|`rem`|relative to the **root** (`html`) font size|
|`vw` / `vh`|1% of the viewport width / height|
|`dvh`|"dynamic" viewport height (follows browser bars on mobile, same as `vh` on desktop)|
|`fr`|fraction of free space (only in CSS Grid)|

**Percentage traps:**
- `height: %` only works if the parent has a **definite height** (not one that depends on
  its own content). Otherwise it is ignored.
- `padding: 1%` is relative to the parent's **width**, even for top and bottom padding. So
  vertical padding changes when the window **width** changes.

### Math functions
```css
width: min(1280px, 100vw);          /* the smallest of the values */
width: max(300px, 50%);             /* the biggest of the values */
width: clamp(300px, 50vw, 800px);   /* min | preferred | max */
height: calc(100vh - 80px);         /* mix units, spaces around + and - are required */
```

### Variables
```css
:root {
    --title-height: 80px;           /* declared once, on the root */
    --main-color: #eef1ff;
}

.mid_center {
    height: calc(100dvh - var(--title-height));
    background-color: var(--main-color);
}
```

## Display and Overflow

### `display`
|Value|Behavior|
|---|---|
|`block`|takes the full width, starts on a new line (`div`, `p`, `h1`)|
|`inline`|flows with the text, **ignores** `width`/`height` (`span`, `a`)|
|`inline-block`|flows like text but accepts `width`/`height`/padding|
|`none`|element removed from the page (takes no space)|
|`flex`|children laid out with Flexbox|
|`grid`|children laid out with CSS Grid|

>**Notes:** `visibility: hidden` hides the element but it **keeps its space**.
>`display: none` removes it completely.

### `overflow`
What happens when the content is bigger than its box:

- `visible` (default): content spills out.
- `hidden`: content is **clipped**, no scrollbar.
- `scroll` / `auto`: scrollbars.

>**Notes:** `overflow: hidden` on `html, body` removes page scrolling, but it also **hides**
>anything that overflows, so layout bugs become invisible. Remove it temporarily when
>debugging.

## Positioning

|Value|Behavior|
|---|---|
|`static`|default, normal flow, `top/left` do nothing|
|`relative`|normal flow, can be nudged; **becomes the reference** for absolute children|
|`absolute`|removed from the flow, positioned against the nearest **positioned ancestor** (not `static`)|
|`fixed`|positioned against the window, stays when scrolling|
|`sticky`|normal flow until a scroll threshold, then sticks|

```css
.stage   { position: relative; }
.button  { position: absolute; top: 5%; right: 5%; }  /* relative to .stage */
```

`z-index` controls which element is drawn on top (only works on positioned elements and
flex/grid items).

## Flexbox
A **one-dimensional** layout: items go along a **main axis**.

```css
.container {
    display: flex;
    flex-direction: row;          /* row (default) | column */
    justify-content: center;      /* alignment on the MAIN axis */
    align-items: center;          /* alignment on the CROSS axis */
    gap: 10px;
    flex-wrap: wrap;              /* allow items to go to the next line */
}
```

- In a `row` container: `justify-content` = horizontal, `align-items` = vertical.
- In a `column` container: **reversed**.
- `align-items: stretch` (default) forces items to fill the cross axis, which can distort
  canvases and images. Use `center` to keep their own size.

### The `flex` shorthand: `flex: grow shrink basis`
|Property|Description|
|---|---|
|`flex-grow`|share of the **extra** space the item takes|
|`flex-shrink`|how much it shrinks when there is **not enough** space|
|`flex-basis`|starting size before growing/shrinking|

```css
.fixed { flex: 0 0 auto; }   /* natural size, never grows or shrinks */
.fill  { flex: 1 1 0; }      /* start at 0, grow to take ALL the remaining space */
```

### `min-width: 0` / `min-height: 0` (very common bug)
- Flex items have `min-width: auto` / `min-height: auto` by default: they **refuse to shrink
  below their content size**.
- If a big element (canvas, image) is inside, its flex parent can't shrink -> overflow.
- Fix: `min-height: 0` (column) or `min-width: 0` (row) on the flex item.

## Grid
A **two-dimensional** layout: rows and columns at the same time.

```css
.stage {
    display: grid;
    grid-template-columns: auto 1fr auto;   /* left buttons | canvas | right buttons */
    grid-template-rows: auto 1fr auto;
    grid-template-areas:
        "top    top    top"
        "left   canvas right"
        "bottom bottom bottom";
    gap: 8px;
}

.canvas { grid-area: canvas; }
```

- `fr` = a fraction of the free space. `1fr` takes what is left.
- `auto-fit` + `minmax()` create responsive grids that reflow by themselves.

## Sizing elements

### `width`, `height`, `min-*`, `max-*`
- `width`/`height`: the size you ask for.
- `min-width`/`min-height`: the size can never go **below** this.
- `max-width`/`max-height`: the size can never go **above** this.
- Always prefer `max-width` over fixed `width` for things that must shrink.

### `aspect-ratio`
```css
aspect-ratio: 16 / 9;   /* height is computed from the width (or the opposite) */
```

### Replaced elements
`canvas`, `img`, `video` are **replaced elements**: they have an **intrinsic size** and an
**intrinsic ratio** (for a canvas, from its `width`/`height` attributes).

- With `width: auto; height: auto` they render at their intrinsic size.
- With `max-width` + `max-height`, the browser shrinks them **while keeping the ratio**.
- A normal `div` does **not** do this: two `max-*` limits do not keep a ratio.

### `border` vs `outline`
- `border` takes layout space and interacts with `box-sizing`, `aspect-ratio` and `max-*`
  (can shift the ratio by a few px).
- `outline` is drawn **outside** the box and takes **no space**. Layout math stays clean.
  It cannot have separate sides.

## Canvas and CSS
Two completely different sizes:

- HTML attributes `width="1280" height="720"` = size of the **drawing bitmap** (the pixel grid
  / your logical coordinates).
- CSS `width` / `height` = size of the **box on screen**.
- If they differ, the browser **stretches** the bitmap.
- Changing the attribute from JS **clears** the canvas.

```css
canvas {
    image-rendering: pixelated;   /* sharp pixels when scaled, instead of blurry */
}
```

>**Notes:** If the canvas is displayed smaller than its bitmap, mouse coordinates must be
>converted (this is JavaScript, see the JS notes):
>```js
>const rect = canvas.getBoundingClientRect();
>const x = (event.clientX - rect.left) * (canvas.width / rect.width);
>const y = (event.clientY - rect.top)  * (canvas.height / rect.height);
>```

## Responsive design
**Strategies** for when the window changes size:

1. **Scale:** element grows/shrinks (`%`, `vw`/`vh`, `max-width`, `clamp()`, `aspect-ratio`).
2. **Reflow:** elements rearrange (`flex-wrap`, Grid `auto-fit`, media queries).
3. **Scroll or hide:** let the page scroll, or hide non-essential elements.

### Media queries
Apply rules only when a condition is true:

```css
@media (max-width: 800px) {
    .sidebar { display: none; }
}

@media (min-width: 1200px) {
    #title { font-size: 40px; }
}
```

>**Notes:** Design the smallest layout first, then add `min-width` rules for bigger screens.

### Container queries
Units and rules based on the size of a **container** instead of the window:

```css
.stage { container-type: inline-size; }
.button { font-size: clamp(8px, 2cqw, 16px); }   /* cqw = 1% of the container width */
```

## Fly-in Simulator: layout case study
**Goal:** the canvas is always fully visible, no scroll, it shrinks with the window, never
grows past 1280x720, desktop only. Buttons will go around the canvas and must move with it.

### Why the first version overflowed
- The canvas has `width="1280" height="720"` and no CSS size: it always rendered at 1280x720.
- `.mid_center` was `80vh`, but the title also takes space -> total is greater than `100vh`.
  `80vh` was a guess, not a rule.
- Default `content-box` added borders and padding on top of the sizes.

### The layout chain
The size must flow from top to bottom. Every level needs a clear size, otherwise the canvas
has nothing to measure against.

```
html           100dvh
 └ body        100dvh, flex column
    ├ .top_center   natural height
    └ .mid_center   ALL remaining height
        └ canvas    max 100% of .mid_center
```

### Final CSS
```css
/* || ------- Custom Fonts ------- */
@font-face {
    font-family: Main_Title;
    src: url(fonts/PressStart2P-Regular.ttf);
}

/* || ------- General Styles ------- */
*,
*::before,
*::after {
    box-sizing: border-box;
}

html,
body {
    margin: 0;
    height: 100dvh;
    overflow: hidden;
}

body {
    display: flex;
    flex-direction: column;
    background-color: #eef1ff;
}

/* || ------- Classes ------- */
.top_center {
    flex: 0 0 auto;          /* natural height, never shrinks */
    text-align: center;
    padding: 8px 0;
}

.mid_center {
    flex: 1 1 0;             /* take all remaining space */
    min-height: 0;           /* allow shrinking below content size */
    min-width: 0;
    display: flex;
    justify-content: center;
    align-items: center;
    padding: 12px;
}

/* || ------- ID ------- */
#title {
    display: inline-block;
    font-family: Main_Title, monospace;
    font-size: 30px;
    padding: 1%;
    margin: 0;
    border: solid grey;
}

#simulation {
    width: auto;
    height: auto;            /* intrinsic 1280x720 = max size */
    max-width: 100%;
    max-height: 100%;
    aspect-ratio: 16 / 9;
    outline: 3px solid grey; /* outline: does not affect the ratio math */
    background-color: transparent;
}
```

### Explanation, line by line
- **`box-sizing: border-box` on `*`:** sizes include padding and border, no hidden extra pixels.
- **`html, body { margin: 0; height: 100dvh; overflow: hidden; }`:** page is exactly the window
  size, the default 8px body margin is removed, no scrollbar can appear. `html` needs the
  height too, because percentages resolve against the parent.
- **`body { display: flex; flex-direction: column; }`:** the title and the middle area are
  stacked vertically and share the window height.
- **`.top_center { flex: 0 0 auto; }`:** keeps the title's natural height.
- **`.mid_center { flex: 1 1 0; min-height: 0; }`:** takes exactly "window height - title
  height". `min-height: 0` lets it shrink even if the canvas inside is big.
- **`.mid_center { display: flex; justify-content: center; align-items: center; }`:** centers
  the canvas horizontally and vertically. `center` (not `stretch`) keeps the canvas ratio.
- **`padding: 12px` on `.mid_center`:** keeps the canvas away from the window edges. Safe
  because of `border-box`.
- **`#title { margin: 0; }`:** removes the default `h1` margins that would add to the height.
- **`#simulation { width: auto; height: auto; }`:** start from the intrinsic 1280x720, so it
  never grows past that.
- **`max-width: 100%; max-height: 100%;`:** shrink to fit the free area. Percentages resolve
  against `.mid_center`'s content box, which has a definite size thanks to the chain above.
- **`aspect-ratio: 16 / 9`:** safety net, almost redundant for a canvas. It matters when the
  sizing moves to a wrapper `div`.
- **`outline` instead of `border`:** the frame does not take layout space.

### If it still doesn't fit
1. `.mid_center` computed height must be smaller than the window (DevTools).
2. Add `min-width: 0; min-height: 0;` on `#simulation`.
3. Remove `overflow: hidden` temporarily to see what overflows.

### Why not the alternatives
- Fixed `80vh` for the middle area: ignores the title height, breaks when the title changes.
- `calc(100vh - Xpx)`: works, but X must be known and updated by hand.
- Absolute positioning everything: not responsive.
- Flex remainder (`flex: 1 1 0`): automatic, no numbers to maintain.

### Future: buttons around or over the canvas
Wrap canvas + buttons in a `.stage` inside `.mid_center` and move the sizing rules to
`.stage`:

- Buttons **around** the canvas: CSS Grid on `.stage` with `grid-template-areas`.
- Buttons **over** the canvas: `.stage { position: relative; }` and buttons
  `position: absolute` with `%` offsets.
- Buttons that **scale** with the stage: container query units (`cqw`) or `clamp()` with `vw`.

>**Notes:** A plain `div` does **not** keep its ratio with two `max-*` limits, unlike a canvas.
>One option to test is
>`width: min(1280px, 100%, calc((100dvh - <title + padding>) * 16 / 9))`.
>This idea is **not tested yet**, verify the real behavior in the browser.


## Resources
- MDN CSS reference: https://developer.mozilla.org/en-US/docs/Web/CSS/Reference
- MDN Box model, Flexbox, Grid, Replaced elements, `aspect-ratio`, `@font-face`
- CSS-Tricks "A Complete Guide to Flexbox" and "A Complete Guide to Grid"
- Canvas drawing properties: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D