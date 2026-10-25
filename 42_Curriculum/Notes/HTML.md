# HTML

This *markdown file* will be used to write down everything related to `HTML` and, such
as syntax methods and other things related to the programming language

## HTML Basics

All HTML documents must start with a `document type declaration` **\<!DOCTYPE html>**.
The HTML document itself begins with **\<html>** and end with **\</html>**.
The visible part of the HTML document is between **\<body>** and **\</body>**.

```html
<!DOCTYPE html>
<html>
<body>

<h1>My First Heading</h1>
<p>My first paragraph.</p>

</body>
</html>
```

### <!DOCTYPE> Declaration
The `\<!DOCTYPE> declaration` represent the document type, and helps the browser display web
pages correctly. It only appears once, at the top of the page (Before any HTML tags).

### Title

The title of the document shown in the **browser's title bar** or in the page's tab is 
defined with the element **\<title>**. This is requirement for HTML documents.

```html
<!DOCTYPE html>
<html>
<head>
  <title>A Meaningful Page Title</title>
</head>
<body>...</body>
</html>
```

You can also add an icon to the left of the browser using a `Favicon`. The image must be on
a folder of the same level as the `index.html` file.

```html
<!DOCTYPE html>
<html>
<head>
  <title>My Page Title</title>
  <link rel="icon" href="/images/favicon.ico">
</head>
<body>...</body>
</html>
```

### Headings
The `headings` in HTML are declared with **\<h1>** to **\<h6>** tags. They go smaller as the
number lowers

```html
<h1>This is heading 1</h1>
<h2>This is heading 2</h2>
<h3>This is heading 3</h3>
```

Each HTML heading has a `default size`. However, you can specify the size for any heading 
with the **style attribute**, using the CSS font-size property:

```html
<h1 style="font-size:60px;">Heading 1</h1>
```

### Paragraphs
`Paragraphs` are defined with the **\<p>** tag.

```html
<p>This is a paragraph.</p>
<p>This is another paragraph.</p>
```

### Links
`Links` are defined with the **\<a>** tag and the link's destination is specified in the `href`
attribute. Attributes are used to provide additional information about HTML elements.

```html
<a href="https://www.w3schools.com">This is a link</a>
```

`HTML links` are **hyperlinks** that you can click and it will send you to another document.

>**Notes:** A link can be text, an image or any other HTML element.

Links have a `target` attribute to specify where you want the link to open up 
*(Default is on the current browser window)*

The target attribute can have one of the following values:

- **_self** - Default. Opens the document in the same window/tab as it was clicked
- **_blank** - Opens the document in a new window or tab
- **_parent** - Opens the document in the parent frame
- **_top** - Opens the document in the full body of the window

To use an image as a link you just need to put the **\<img>** tag inside the **\<a>**.

```html
<a href="default.asp">
<img src="smiley.gif" alt="HTML tutorial" style="width:42px;height:42px;">
</a>
```

### Images
HTML `images` are defined with the **\<img>** tag. The source file (src), alternative text 
(alt), width, and height are provided as attributes

```html
<img src="w3schools.jpg" alt="W3Schools.com" width="104" height="142">
```

## Elements
HTML `elements` can be nested which means that elements can contain other elements. Elements
can be (\<html>, \<body>, \<h1> or \<p>).
The root element of a document is the \<html> and defines the whole document.
>You must never forget the closing tag since this could lead to unexpected results !!

HTML also has elements with no content called `empty elements` such as **\<br>** which 
defines a `\n`

```html
<p>This is a <br> paragraph with a line break.</p>
```

Also, HTML isn't case-sensitive so you could write them upper-case or lower-case with no
conflict.

Other types of elements:

|Tag|Description|
|---|---|
|\<p>|Defines a paragraph|
|\<hr>|Defines a thematic change in the content|
|\<br>|Inserts a single line break|
|\<pre>|Defines pre-formatted text|

## Attributes
HTML `attributes` provide additional information about `HTML elements`.

- All HTML elements can have attributes
- Attributes provide additional information about elements
- Attributes are always specified in the start tag
- Attributes usually come in name/value pairs like: name="value"

Some examples of attributes:

```html
<!--The <a> tag defines a hyperlink. The href attribute specifies the URL of the page the 
link goes to-->
<a href="https://www.w3schools.com">Visit W3Schools</a>

<!--The <img> tag is used to embed an image in an HTML page. The src attribute specifies the path to the image to be displayed-->
<img src="img_girl.jpg">
```

There are 2 ways of specifying a `URL` in the **src** attribute:
1. **Absolute URL:** Links to an external image that is hosted on another website.
2. **Relative URL:** Links to an image that is hosted within the website.

>**Notes:** It is almost always best to use relative URLs. They will not break if you change 
>domain.

```html
<!-- The width and height attributes, which specify the width and height of the image (in pixels)-->
<img src="img_girl.jpg" width="500" height="600">

<!--The required alt attribute for the <img> tag specifies an alternate text for an image, if the image for some reason cannot be displayed-->
<img src="img_girl.jpg" alt="Girl with a jacket">

<!--The style attribute is used to add styles to an element, such as color, font, size, and more-->
<p style="color:red;">This is a red paragraph.</p>
```

The `lang` attribute is used for the **\<html>** elements, and it declares the language of 
the Web page. This is made to assist search engines and browsers.

```html
<!DOCTYPE html>
<html lang="en">
<body>
...
</body>
</html>
```

Country codes can also be added to the language code in the `lang` attribute. So, the first 
two characters define the language of the HTML page, and the last two characters define the 
country *(e.g. \<html lang="en-US">)*

## Styles
The `HTML style` attribute is used to add styles to an element, such as **color**, **font**, 
**size**, and more.

The HTML style attribute has the following syntax:

```html
<tagname style="property:value;">
```

>**Notes:** The property is a CSS property. The value is a CSS value.

**Background Color**

The CSS `background-color property` defines the background color for an HTML element.

Example:
- Set the background color for a page to *powder blue*:

```html
<body style="background-color:powderblue;">

<h1>This is a heading</h1>
<p>This is a paragraph.</p>

</body>
```

**Text Color**

The `CSS color property` defines the text color for an HTML element:

```html
<h1 style="color:blue;">This is a heading</h1>
<p style="color:red;">This is a paragraph.</p>
```

**Fonts**

The `CSS font-family property` defines the font to be used for an HTML element:

```html
<h1 style="font-family:verdana;">This is a heading</h1>
<p style="font-family:courier;">This is a paragraph.</p>
```

**Text Size**

The `CSS font-size property` defines the text size for an HTML element:

```html
<h1 style="font-size:300%;">This is a heading</h1>
<p style="font-size:160%;">This is a paragraph.</p>
```

**Text Alignment**

The `CSS text-align property` defines the horizontal text alignment for an HTML element:

```html
<h1 style="text-align:center;">Centered Heading</h1>
<p style="text-align:center;">Centered paragraph.</p>
```

## Formatting
HTML contains several elements for defining text with a special meaning.

Formatting elements were designed to display special types of text:

- \<b> - Bold text
- \<strong> - Important text
- \<i> - Italic text
- \<em> - Emphasized text
- \<mark> - Marked text
- \<small> - Smaller text
- \<del> - Deleted text
- \<ins> - Inserted text
- \<sub> - Subscript text
- \<sup> - Superscript text

## Colors
HTML `colors` are specified with predefined **color names**, or with **RGB**, **HEX**, 
**HSL**, **RGBA**, or **HSLA** values.

There are up to 140 supported color names. [HTML Color Names](https://www.w3schools.com/colors/colors_names.asp)

You can add color to text with `style="color:Tomato;"`, background color with 
`style="background-color:DodgerBlue;"` or border color with `style="border:2px solid Tomato;"`

## CSS (Cascading Style Sheets)
`CSS` is used to format the layout of a webpage. With CSS, you can control the color, font, 
the `size of text`, the `spacing between elements`, how elements are positioned and laid 
out, what `background images` or `background colors` are to be used, different displays for 
different devices and screen sizes, and much more!

There are 3 ways of using CSS within HTML:

1. **Inline CSS:**

Using inline CSS you apply a unique style to a single HTML element. We use this with the
**style attribute** of an element.

```html
<h1 style="color:blue;">A Blue Heading</h1>
<p style="color:red;">A red paragraph.</p>
```

2. **Internal CSS**

An internal CSS is used to define a style for a single HTML page. An internal CSS is defined in the **\<head>** section of an HTML page, within a **\<style> element**.

```html
<!DOCTYPE html>
<html>
<head>
<style>
body {background-color: powderblue;}
h1   {color: blue;}
p    {color: red;}
</style>
</head>
<body>

<h1>This is a heading</h1>
<p>This is a paragraph.</p>

</body>
</html>
```
3. **External CSS**
An `external style` sheet is used to define the style for many HTML pages. To use an external style sheet, add a **link** to it in the **\<head>** section of each HTML page

```html
<!DOCTYPE html>
<html>
<head>
  <link rel="stylesheet" href="styles.css">
</head>
<body>

<h1>This is a heading</h1>
<p>This is a paragraph.</p>

</body>
</html>
```

The external style sheet can be written in any text editor. The file must not contain any 
HTML code, and must be saved with a .css extension.

Here is what the "styles.css" file looks like:

```css
body {
  background-color: powderblue;
}
h1 {
  color: blue;
}
p {
  color: red;
}
```

>**Notes:** Tip: With an external style sheet, you can change the look of an entire website, by changing one file!

Here is a list of some common CSS properties:
- `color`: the text color.
- `font-family`: the font to use.
- `font-size`: the text size.
- `border`: a border around an HTML element.
- `padding`: the space between the text and the border.
- `margin`: the space outside the border.

## Containers \<div>
The `<div>` element is by default a **block element**, meaning it takes all available width,
and comes with line breaks before and after. It doesn't come with mandatory attributes, but is
often used with `style`, `class`, and `id`.

The `<div>` element is often used to group sections of a web page together.

```html
<div>
  <h2>London</h2>
  <p>London is the capital city of England.</p>
  <p>London has over 9 million inhabitants.</p>
</div>
```

You can center-align a *\<div>* element using CSS `margin` property.

```html
<style>
div {
  width:300px;
  margin:auto;
}
</style>
```

## Classes
The HTML `class` attribute is used to specify a class for an HTML element. Multiple HTML 
elements can share the same class. The `class` attribute is often used to point to a 
class name in a style sheet.

In the following example three `div` elements will be styled equally according to the
`.city` style definition since they all share the same class.

```css
.city {
  background-color: tomato;
  color: white;
  border: 2px solid black;
  margin: 20px;
  padding: 20px;
}
```

```html
<!DOCTYPE html>
<html>
<body>

<div class="city">
  <h2>London</h2>
  <p>London is the capital of England.</p>
</div>

<div class="city">
  <h2>Paris</h2>
  <p>Paris is the capital of France.</p>
</div>

<div class="city">
  <h2>Tokyo</h2>
  <p>Tokyo is the capital of Japan.</p>
</div>

</body>
</html>
```

The `class` attribute can be used on **any** HTML element.

## ID
The HTML `id` attribute is used to specify a unique **id** for an HTML element. The id 
attribute is used to **point to a specific style declaration** in a style sheet. It is 
also used by JavaScript to access and manipulate the element with the specific id.

The syntax for id is: write a hash character `(#)`, followed by an id name. Then, define 
the CSS properties within curly braces `{}`.

```html
<!DOCTYPE html>
<html>
<head>
<style>
#myHeader {
  background-color: lightblue;
  color: black;
  padding: 40px;
  text-align: center;
}
</style>
</head>
<body>

<h1 id="myHeader">My Header</h1>

</body>
</html>
```

## Buttons
To create buttons you use the **\<button>** elements. This will create a clickable 
button. On its own it does nothing, so some functionality must be added.

Buttons are often styled with CSS.

```html
<!DOCTYPE html>
<html>
<head>
<style>
.testbtn {
  background-color: #04AA6D;
  color: white;
  font-size: 15px;
  padding: 12px;
  border: none;
  border-radius: 5px;
  cursor: pointer;
}

.testbtn:hover {
  background-color: #059862;
}
</style>
</head>
<body>

<h1>HTML Buttons</h1>

<p>Default button with no styling:</p>
<button>Click me</button>

<p>Styled button:</p>
<button class="testbtn">Green Button</button>

</body>
</html>
```

Buttons can be used with `JavaScript`:

```html
<button onclick="alert('Hello!')">Click Me</button>
```

## JavaScript in HTML

The HTML `\<script>` tag is used to define a **client-side script (JavaScript)**. The 
`\<script>` element either contains script statements, or it points to an external script 
file through the **src** attribute.

Common uses for JavaScript are **image manipulation**, **form validation**, and **dynamic changes** of content.
To select an HTML element, JavaScript most often uses the `document.getElementById()` 
method.

```html
<script>
document.getElementById("demo").innerHTML = "Hello JavaScript!";
</script>
```

## Canvas
The HTML **\<canvas>** element is used to `draw graphics`, on the fly, via JavaScript. 
This element is only a `container for graphics`. You **MUST** use JavaScript to actually
draw the graphics. The **Canvas API** allows JavaScript to draw on the canvas, and has 
several methods for drawing paths, boxes, circles, text, and adding images.

```html
<canvas id="myCanvas" width="300" height="150"></canvas>
```

