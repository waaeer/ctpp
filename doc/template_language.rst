========================================
The CT++ template language (2.8 version)
========================================

.. contents::
    :depth: 2

.. role:: tmpl
.. role:: const
.. role:: arg
.. role:: func

Templates
=========

    A template is a text file, that is marked-up using the CT++ template
    language. It used to separate content from presentation in web design.

    A template can contain CT++ tags. All CT++ tags have HTML-like syntax and
    they start with :tmpl:`TMPL_`. Names of CT++ tags are case insensetive (for
    example, :tmpl:`TMPL_var`, :tmpl:`tmpl_var`, or :tmpl:`TmPl_VaR`).

    CT++ supports the next tags: :tmpl:`TMPL_var`, :tmpl:`TMPL_if`,
    :tmpl:`TMPL_elsif`, :tmpl:`TMPL_else`, :tmpl:`TMPL_unless`,
    :tmpl:`TMPL_foreach`, :tmpl:`TMPL_include`, :tmpl:`TMPL_comment`,
    :tmpl:`TMPL_block`, :tmpl:`TMPL_call`, and :tmpl:`TMPL_verbose`.

    Example of a template:

    .. parsed-literal::

            <!DOCTYPE html>
            :tmpl:`<TMPL_include 'includes/html_lang.tmpl'>`
            :tmpl:`<TMPL_include 'includes/html_head.tmpl'>`
                <link rel="stylesheet" href="/static/css/m.posts.css">
            </head>
            <body class="posts">
                <div id="body-wrapper" class="body-wrapper">
                    <div id="content-wrapper" class="content-wrapper">
                        :tmpl:`<TMPL_include 'includes/header.tmpl'>`
                        <section role="main" class="content-column">
                            <h1>\ :tmpl:`<TMPL_var _("Welcome to")>` :tmpl:`<TMPL_var project_name>`, :tmpl:`<TMPL_verbose>`
                            :tmpl:`<TMPL_if logged>`
                                :tmpl:`<TMPL_var HTMLESCAPE(DEFAULT(user_info.name, user_info.username))>`
                            :tmpl:`<TMPL_else>`
                                :tmpl:`<TMPL_var _("Anonymous")>`
                            :tmpl:`</TMPL_if>`\ </h1>
                            :tmpl:`</TMPL_verbose>`

                            :tmpl:`<TMPL_verbose>`
                            :tmpl:`<TMPL_foreach users as user>`
                                <h3><a class="user-page-link" href="/user/:tmpl:`<TMPL_var URLESCAPE(user.username)>`" title="
                                    :tmpl:`<TMPL_var HTMLESCAPE(DEFAULT(user.name, user.username))>">`
                                    :tmpl:`<TMPL_var HTMLESCAPE(DEFAULT(user.name, user.username))>`
                                    </a></h3>
                            :tmpl:`</TMPL_foreach>
                            </TMPL_verbose>`

                        </section>
                        :tmpl:`<TMPL_include 'includes/noscript_alert.tmpl'>`
                    </div>
                </div>
                :tmpl:`<TMPL_include 'includes/footer.tmpl'>`
            </body>
            </html>

Comments
--------

    Use the :tmpl:`TMPL_comment` tag to comment-out part of a template.

    For example:

    .. parsed-literal::

        :tmpl:`<TMPL_comment>`\ <h3>Hello, :tmpl:`<TMPL_var username>`\ !</h3>\ :tmpl:`</TMPL_comment>`

Formatting
----------

    The :tmpl:`TMPL_verbose` tag removes white-space characters (space,
    form-feed, newline, carriage return, horizontal tab, and vertical tab)
    between CT++ tags.

    For example:

    .. parsed-literal::

        :tmpl:`<TMPL_verbose>`
            foo: :tmpl:`<TMPL_var foo>` some text
            bar: :tmpl:`<TMPL_var bar>` baz: :tmpl:`<TMPL_var baz>` :tmpl:`<TMPL_var spam>`
        :tmpl:`</TMPL_verbose>`

    For instance, variables foo, bar, baz, and spam are respectively
    :const:`"{FOO}"`, :const:`"{BAR}"`, :const:`"{BAZ}"`, and
    :const:`"{SPAM}"`. The template would be rendered as::

        foo:{FOO}some text
            bar:{BAR}baz:{BAZ}{SPAM}

    Also, a dash could be used instead the :tmpl:`TMPL_verbose` tag:

    .. parsed-literal::

        foo: :tmpl:`<TMPL_var foo->` some text
            bar: :tmpl:`<-TMPL_var bar->` baz: :tmpl:`<-TMPL_var baz->` :tmpl:`<-TMPL_var spam>`

    There will same output.

Including
---------

    The :tmpl:`TMPL_include` tag loads another template within the template.
    The tag has the one attribute only. The attribute can't be a variable.
    It's a filename in quotes::

        <TMPL_include "filename.tmpl">

    It is impossible to break conditions and loops between templates.

Variables
=========

    The CT++ template engine evaluates variable and replaces it with the
    result. The :tmpl:`TMPL_var` tag is used for evaluating variables.

    The types of variables are undefined variable, string, integer,
    floating-point number, mapping, and array.  For undefined variables result
    is an empty string.

    The :tmpl:`TMPL_var` evaluates expressions: variables, operators, and
    functions. For example::

        <TMPL_var foo>
        <TMPL_var (foo + bar + 1)>
        <TMPL_var HTMLESCAPE(baz)>

Arrays
------

     An array is a data type consisting of a collection of elements, each
     identified by one array index.  The first element of the array is indexed
     by subscript of 0.

     Examples::

        <TMPL_var foo[0]>   <- foo must be an array!
        <TMPL_var foo[bar]> <- bar must be an integer!

Mappings
--------

    A mapping is a data type composed of a collection of (key,value)
    pairs, such that each possible key appears at most once in the collection.

    Examples::

        <TMPL_var foo.key> = <TMPL_var foo["key"]> <- foo must be a mapping
        <TMPL_var foo.bar.baz> <- the mapping foo has the key "bar" with mapping value, that has the key "baz"

    Use a dot to access attributes of a variable.

Control flow
============

Choice
------

    The :tmpl:`TMPL_if` tag and the :tmpl:`TMPL_unless` tag evaluate a
    condition.  A condition can be variable, function calls, arithmetic and
    logic expressions.  Arithmetic and logic expressions must be inside of
    parentheses.

    Examples:

    .. parsed-literal::

        :tmpl:`<TMPL_if foo>`
            Some text with :tmpl:`<TMPL_var foo>`
        :tmpl:`</TMPL_if>`

        :tmpl:`<TMPL_unless DEFINED(bar)>`
            Some text
        :tmpl:`<TMPL_else>`
            Some text with :tmpl:`<TMPL_var bar>`
        :tmpl:`</TMPL_unless>`

        :tmpl:`<TMPL_if (x < 1)>`
            eeny
        :tmpl:`<TMPL_elsif (x < 2)>`
            meeny
        :tmpl:`<TMPL_elsif (x < 3)>`
            miny
        :tmpl:`<TMPL_else>`
            moe
        :tmpl:`</TMPL_if>`

    The following table summarizes the operator precedences, from highest
    precedence to lowest precedence:

    =============== ===== ====================================
    Operators       Assoc Description
    =============== ===== ====================================
    (expression)          parentheses used for grouping
    --------------- ----- ------------------------------------
    ()              LR    parentheses used for a function call
    --------------- ----- ------------------------------------
    .               LR    member selection 
    --------------- ----- ------------------------------------
    []              LR    array/mapping element access
    --------------- ----- ------------------------------------
    !, +, -         RL    logical not, unary plus, unary minus
    --------------- ----- ------------------------------------
    \*, /, mod, div LR    multiplication and division 
    --------------- ----- ------------------------------------
    +, -            LR    addition and subtraction
    --------------- ----- ------------------------------------
    <, <=, >, >=    LR    inequality relational
    --------------- ----- ------------------------------------
    ==, !=          LR    equality relational
    --------------- ----- ------------------------------------
    &&              LR    logical and
    --------------- ----- ------------------------------------
    ||              LR    logical or
    =============== ===== ====================================

    CT++ defines keywords to act as aliases for a number of operators: lt (<),
    le (<=), gt (>), ge (>=), eq (==), ne (!=), and (&&), or (||).

Loops
-----

    CT++ supports the :tmpl:`TMPL_foreach` only.   It use an explicit iterator,
    in which the loop variable takes on each of the values in an array or a
    mapping.

    An iterator has special attributes:

    ========= ============================================
    Attribute Description
    ========= ============================================
    __index__ index of array's element
    --------- --------------------------------------------
    __key__   key in mapping
    --------- --------------------------------------------
    __first__ "true" for first iteration
    --------- --------------------------------------------
    __last__  "true" for last iteration
    --------- --------------------------------------------
    __inner__ "true" if not first and not last iteration
    --------- --------------------------------------------
    __even__  "true" for even iteration
    --------- --------------------------------------------
    __odd__   "true" for odd iteration
    --------- --------------------------------------------
    __value__ value of element
    ========= ============================================

    Example of usage:

    .. parsed-literal::

        :tmpl:`<TMPL_foreach array as a>`
            value: :tmpl:`<TMPL_var a>` or :tmpl:`<TMPL_var a.__value__>`
            index: :tmpl:`<TMPL_var a.__index__>`
            :tmpl:`<TMPL_if a.__first__>`\ #first#\ :tmpl:`</TMPL_if>`
            :tmpl:`<TMPL_if a.__even__>`\ #even#\ :tmpl:`<TMPL_else>`\ #odd#\ :tmpl:`</TMPL_if>`
        :tmpl:`</TMPL_foreach>`

        :tmpl:`<TMPL_foreach map as m>`
            value: :tmpl:`<TMPL_var m>` or :tmpl:`<TMPL_var m.__value__>`
            key: :tmpl:`<TMPL_var m.__key__>`
            :tmpl:`<TMPL_if m.__last__>`\ #last#\ :tmpl:`</TMPL_if>`
        :tmpl:`</TMPL_foreach>`

    The :tmpl:`TMPL_break` tag breaks the iteration of loop::

        <TMPL_verbose>
        <TMPL_foreach LIST("a", "b", "c") as char>
            <TMPL_var char>
            <TMPL_if (char == "b")><TMPL_break></TMPL_if>
        </TMPL_foreach>
        </TMPL_verbose>

    Output of the template is "ab".

Functions
=========

    CT++ has built-in functions, that is described in `Library reference`_.
    CT++ tags, that support it: :tmpl:`TMPL_var`, :tmpl:`TMPL_if`,
    :tmpl:`TMPL_unless`, :tmpl:`TMPL_elsif`, and :tmpl:`TMPL_foreach`.  For
    example:

    .. parsed-literal::

        <TMPL_var :func:`SPRINTF`\ (\ :func:`GETTEXT`\ ("%d minutes ago", min), min)>

        <TMPL_var :func:`DEFAULT`\ (\ :func:`HTMLESCAPE`\ (url), "http://ctpp.havoc.ru")>

        <TMPL_if (\ :func:`GET_TYPE`\ (var) == "STRING")>
            <TMPL_var :func:`GETTEXT`\ ("var is string")>
        </TMPL_if>

        <TMPL_foreach :func:`LIST`\ ("eeny", "meeny", "miny", "moe") as item>
            <TMPL_var item>
        </TMPL_foreach>

Blocks
======

    A block is reusable piece of a template called by name.

    Example of usage:

    .. parsed-literal::

        :tmpl:`<TMPL_block "simple-block">`
            Some text
        :tmpl:`</TMPL_block>`

    The :tmpl:`TMPL_call` tag is used for calling a block:

    .. parsed-literal::

        :tmpl:`<TMPL_call "simple-block">`
        :tmpl:`<TMPL_call some_var>`

    A block can have arguments:

    .. parsed-literal::

        :tmpl:`<TMPL_block 'some-block' args(a, b, c)>`
            :tmpl:`<TMPL_var a>`\ , :tmpl:`<TMPL_var b>`\ , and :tmpl:`<TMPL_var c>`
        :tmpl:`</TMPL_block>`

        :tmpl:`<TMPL_call 'some-block' args(foo, bar, baz)>`

    Definition blocks inside another blocks is forbidden.

.. _`Library reference`:

Library reference
=================

_ *(msgid[, msgid_plural, n][, domain])*
----------------------------------------

See `GETTEXT`_.

AVG *(flag, a[, b, ...])*
-------------------------

    Return average of :arg:`a[, b, ...]`, according to chosen algorithm by
    :arg:`flag`.  The values of :arg:`flag` are :const:`'a'` for arithmetic mean,
    :const:`'g'` for geometric mean, :const:`'h'` for harmonic mean, and :const:`'q'`
    for quadratic mean.

    Examples::

        AVG('a', 1, 2, 3) -> 2
        AVG('g', 1, 2, 3) -> 1.81712059283
        AVG('h', 1, 2, 3) -> 1.63636363636
        AVG('q', 1, 2, 3) -> 2.16024689947


BASE64_DECODE *(x)*
-------------------

    Decode the Base64 encoded string :arg:`x`.


BASE64_ENCODE *(x)*
-------------------

    Encode the string :arg:`x` use Base64.

CAST *(flag, x)*
----------------

    Convert the type of :arg:`x` to a type specified by :arg:`flag`.  The
    values of :arg:`flag` are :const:`"i[nteger]"`, :const:`"o[ctal]"`,
    :const:`"h[exadecimal]"`, :const:`"f[loat]"`, and :const:`"s[tring]"`.

    For :arg:`x` starts with :const:`0x` or :const:`0X`, :const:`"integer"`
    :arg:`flag` behaves as :const:`"hexadecimal"` :arg:`flag`.  For :arg:`x`
    starts with :const:`0`, :const:`"integer"` :arg:`flag` behaves as
    :const:`"octal"` :arg:`flag`.

    Examples::

        CAST("int", 1.345) -> 1
        CAST("dec", "010") -> 10
        CAST("hex", "010") -> 16

CONCAT *(a[, b, ...])*
----------------------

    Return a string which is the concatenation of the values :arg:`a[, b,
    ...]`.

    Example::

        CONCAT('a', 1, 2, 3) -> a123

CONTEXT *()*
------------

    Return a mapping of the rendered context.

DATE_FORMAT *(x, format)*
-------------------------

    Formats :arg:`x` according to the format specification :arg:`format` (see
    'man 3 strftime').  The argument :arg:`x` is the number of seconds elapsed
    since the Epoch, 1970-01-01 00:00:00 +0000 (UTC). 

    Example::

        DATE_FORMAT(1200490323, "%Y-%m-%d %H:%M:%S") -> 2008-01-16 16:32:03


DEFAULT *(x, y)*
----------------------

    If :arg:`x` is nonzero return :arg:`x` else return :arg:`y`.

    Examples::

        DEFAULT("", "default_string")   -> default_string
        DEFAULT(nonexistent, "default") -> default
        DEFAULT(0, 1)                   -> 1

DEFINED *(a[, b, ...])*
-----------------------

    If all of :arg:`a[, b, ...]` exist return :const:`1` else return
    :const:`0`.

ERROR *()*
----------

    Create runtime error.

FORM_PARAM *(x, y)*
-------------------

    With name :arg:`x` and value :arg:`y`, output a hidden field of HTML form.

    It is equivalent to the next::

        <TMPL_verbose>
        <TMPL_if y>
            <input type="hidden" name="x" value="<TMPL_var URLESCAPE(y)>">
        </TMPL_if>
        </TMPL_verbose>

.. _GETTEXT:

GETTEXT *(msgid[, msgid_plural, n][, domain])*
----------------------------------------------

    Translate a text string :arg:`msgid` into the user's native language, by
    looking up the translation in a message catalog.

    If a translation was found, the message is converted to the locale's
    codeset and returned. Otherwise :arg:`msgid` or :arg:`msgid_plural` is
    returned.

    The appropriate plural form depends on the number :arg:`n` and the language
    of the message catalog where the translation was found.

    The argument :arg:`domain` is a set of translatable messages.

    The function :func:`_()` is alias of :func:`GETTEXT()`.

    Examples::

        GETTEXT("Hello, World!")                                -> Привет, Мир!
        _("Hello, World!")                                      -> Привет, Мир!
        _("Hello, World!", "mydomain")                          -> Привет, Другой Мир!
        SPRINTF(_("%d hour", "%d hours", 1), 1)                 -> 1 час
        SPRINTF(_("%d minute", "%d minutes", 2, "mydomain"), 2) -> 2 минуты

GET_TYPE *(x)*
--------------

    Return a printable type of :arg:`x`.  The types are :const:`"\*UNDEF\*"` for
    undefined variables, :const:`"STRING"` for strings, :const:`"INTEGER"` for
    integers, :const:`"REAL"` for floating-point numbers, :const:`"HASH"` for
    mappings, and :const:`"ARRAY"` for arrays.

HASH_KEYS *(x)*
------------------

    Return an array of :arg:`x`'s keys.  The argument :arg:`x` should be
    mapping.

HMAC_MD5 *(x, key)*
-------------------------

    Compute a hash value for the specified :arg:`x` using the supplied :arg:`key`.

HOSTNAME *()*
-------------

    Return the hostname of the current processor.

HREF_PARAM *(x, y)*
--------------------------

    With name :arg:`x` and value :arg:`y`, output parameters of HTML links.

    It is equivalent to the next::

        <TMPL_if a>x=<TMPL_var URLESCAPE(y)></TMPL_if>

HTMLESCAPE *(a[, b, ...])*
--------------------------

    Escape :const:`&`, :const:`<`, :const:`'`, :const:`"`, and :const:`>` in
    the strings of data :arg:`a[, b, ...]`.

    :func:`HTMLESCAPE()` returns result of concatenation of the strings
    :arg:`a[, b, ...]`.

ICONV *(x, src, dst[, flags])*
------------------------------

    Convert the string :arg:`x` from the coded character set :arg:`src` to
    :arg:`dst`.

    The argument :arg:`flags` are :const:`'i'` or :const:`'I'` for discarding
    illegal sequence, and :const:`'t'` or :const:`'T'` for enabling transliteration
    in the conversion.  The argument :arg:`flags` works in FreeBSD only.

    See 'iconv -l` for list which contains all the coded character sets known.

IN_ARRAY *(x, array)*
---------------------

    Return :const:`1` if :arg:`x` was found in :arg:`array`, else return
    :const:`0`.

IN_SET *(x, a[, b, ...])*
-------------------------

    Return :const:`1` if :arg:`x` was found in :arg:`a[, b, ...]`, else return
    :const:`0`.

JSONESCAPE *(a[, b, ...])*
--------------------------

    Escape :const:`"`, :const:`\\`, :const:`/`, :const:`\\b`, :const:`\\f`,
    :const:`\\n`, :const:`\\r`, :const:`\\t`, :const:`'`, :const:`\\v`, and
    :const:`\\0` in the strings of data :arg:`a[, b, ...]`.  If character's
    value is smaller than :const:`\\u0032`, it is encoded with :const:`\\uXXXX`
    format for hex digits :const:`XXXX`.

    :func:`JSONESCAPE()` returns result of concatenation of the strings
    :arg:`a[, b, ...]`.

JSON *(x)*
----------

    Serialize :arg:`x` to the JavaScript Object Notation (JSON).

LIST_ELEMENT *(a[, b, ...], x)*
-------------------------------

    Return an element from :arg:`a[, b, ...]` by the index :arg:`x`.

LIST *([a, b, ...])*
--------------------

    Return an array of :arg:`[a, b, ...]`. It can be empty.

LOG *(x[, base])*
---------------------

    With one argument, return the natural logarithm of :arg:`x` (to base
    :const:`e`).

    With two arguments, return the logarithm of :arg:`x` to the given
    :arg:`base`, calculated as :func:`LOG(x) / LOG(base)`.

MAX *(a[, b, ...])*
-------------------

    Return the largest item of :arg:`a[, b, ...]`.

MB_SIZE *(x)*
-------------

    MB\_ prefix marks function, which works with multibyte strings.

    See SIZE_.

MB_SUBSTR *(x, offset[, bytes[, y]]])*
--------------------------------------------

    MB\_ prefix marks function, which works with multibyte strings.

    See SUBSTR_.

MB_TRUNCATE *(x, offest[, addon])*
----------------------------------

    MB\_ prefix marks function, which works with multibyte strings.

    See TRUNCATE_.

MD5 *(a[, b, ...])*
-------------------

    Return the MD5 hash of :arg:`a[, b, ...]`.

MIN *(a[, b, ...])*
-------------------

    Return the smallest item of :arg:`a[, b, ...]`.

NUM_FORMAT *(x, y)*
------------------------

    Return the formatted integer :arg:`x` with period :arg:`y`.

    Example::

        NUM_FORMAT(1234567, ",") -> 1,234,567

OBJ_DUMP *([a, b, ...])*
------------------------

    Return dump of the arguments :arg:`[a, b, ...]`.  Return dump of all variables
    if no arguments.

RANDOM *([[min,] max])*
-----------------------

    Return a random number in the range (:arg:`min`, :arg:`max`].  The range
    (:arg:`min`, :arg:`max`] equals to :const:`0 .. RAND_MAX`, if no arguments. 

.. _SIZE:

SIZE *(x)*
----------

    Return the number of items of a sequence :arg:`x`.

SPRINTF *(format, a[, b, ...])*
-------------------------------

    Produce output according to a format.  After the argument :arg:`format`, it
    expects at least as many additional arguments as specified in
    :arg:`format`.

    The argument :arg:`format` can optionaly contain embedded format tags that
    are substituted by the values specified in :arg:`a[, b, ...]`.

    See 'man 3 printf' for more information.

.. _SUBSTR:

SUBSTR *(x, offset[, bytes[, y]]])*
-----------------------------------

    Return the characters in the string :arg:`x` starting at a specified
    position :arg:`offset` and continuing for a specified number of characters
    :arg:`bytes`.  If the argument :arg:`y` exists, then return the string
    :arg:`x` with replaced the characters with :arg:`y`.

.. _TRUNCATE:

TRUNCATE *(x, offest[, addon])*
-------------------------------

    Return the string :arg:`x`, if it is less than or equal to :arg:`offset`
    characters long.  If it is longer, it truncates the string.

    Return the string with :arg:`addon`, if the argument :arg:`addon` exists.

URIESCAPE *(a[, b, ...])*
-------------------------

    Convert characters of the strings :arg:`a[, b, ...]` into their
    corresponding escape sequences.

    :func:`URIESCAPE()` returns result of concatenation of the strings
    :arg:`a[, b, ...]`.

URLESCAPE *(a[, b, ...])*
-------------------------

    Convert characters of the strings :arg:`a[, b, ...]` into their
    corresponding escape sequences.  It replaces spaces with :const:`"+"` instead
    of :const:`"%20"`.

    :func:`URLESCAPE()` returns result of concatenation of the strings
    :arg:`a[, b, ...]`.

VERSION *([x])*
---------------

    Return the current version of CT++.

    Return extended information if :arg:`x` equals to :const:`"full"`.

WMLESCAPE *(a[, b, ...])*
-------------------------

    Replace :const:`<` with :const:`&gt;`, :const:`>` with :const:`&lt;`,
    :const:`"` with :const:`&quot;`, :const:`'` with :const:`&apos;`,
    :const:`$` with :const:`$$`, and :const:`&` with :const:`&amp;` in the
    strings of data :arg:`a[, b, ...]`.

    :func:`WMLESCAPE()` returns result of concatenation of the strings
    :arg:`a[, b, ...]`.

XMLESCAPE *(a[, b, ...])*
-------------------------

    Replace :const:`<` with :const:`&gt;`, :const:`>` with :const:`&lt;`,
    :const:`"` with :const:`&quot;`, :const:`'` with :const:`&apos;`, and
    :const:`&` with :const:`&amp;` in the strings of data :arg:`a[, b, ...]`.

    :func:`XMLESCAPE()` returns result of concatenation of the strings
    :arg:`a[, b, ...]`.

