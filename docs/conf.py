# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import lxml.etree as ET
import os
import sys
# sys.path.insert(0, os.path.abspath('.'))

#from recommonmark.parser import CommonMarkParse


# -- Project information -----------------------------------------------------

project = 'Cleaver'
copyright = '2020, Scientific Computing and Imaging Institute, University of Utah'
author = 'Jonathan Branson, Brig Bagley, Jess Tate, Ally Warner, Dan White, Ross Whitaker'

# The full version, including alpha/beta/rc tags
release = 'v2.4'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
#extensions = [
#    'sphinx.ext.autodoc',
#    'recommonmark',
#    'sphinx_markdown_tables',
#    'notfound.extension'
#]

extensions = [
    'sphinx.ext.autodoc',
    'myst_parser',
    'sphinx_markdown_tables',
    'notfound.extension'
]

myst_enable_extensions = [
    "colon_fence",  # Allow code fence using ::: (see https://myst-parser.readthedocs.io/en/latest/using/syntax-optional.html#syntax-colon-fence)
    "linkify",  # Allow automatic creation of links from URLs (it is sufficient to write https://google.com instead of <https://google.com>)
]

# Auto-generate header anchors up to level 6, so that it can be referenced like [](file.md#header-anchor).
# (see https://myst-parser.readthedocs.io/en/latest/using/syntax-optional.html#auto-generated-header-anchors)
myst_heading_anchors = 6


# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# the master toctree doc
master_doc = 'index'

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = 'c++'

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

source_suffix = {
    '.rst': 'restructuredtext',
    '.md': 'markdown',
}

notfound_context = {
    'title': 'Page Not Found',
    'body': '''
<h1>Page Not Found</h1>
<p>Sorry, we couldn't find that page.</p>
<p>Try using the search box or go to the homepage.</p>
''',
}


# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'

# If true, `todo` and `todoList` produce output, else they produce nothing.
todo_include_todos = False

# A string of reStructuredText that will be included at the beginning of every source file that is read.
#rst_prolog = open('global.rst.in').read()

html_logo = '_static/images/splash.png'

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#

html_theme = 'default'

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#
html_theme_options = {
    # Toc options
    'includehidden': False,
}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

on_rtd = os.environ.get('READTHEDOCS', None) == 'True'

if not on_rtd:  # only import and set the theme if we're building docs locally
    import sphinx_rtd_theme
    html_theme = 'sphinx_rtd_theme'
    html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]
  
  
#latex_documents = [
#    (master_doc, '3DSlicer.tex', '3D Slicer Documentation',
#     'Slicer Community', 'manual'),
#]

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']
