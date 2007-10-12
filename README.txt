$Id$

The Primary Term modules allows you to specify which taxonomy term
should be the "primary" term for a node. For nodes that can have
multiple terms from a single vocabulary, a primary term can specify
which summary page the node should be listed on, which theme template
to apply, etc.

NOTE: I adopted this module in October, 2007. I am maintaining the
Drupal 5 and 6 versions, but not the Drupal 4.7 version. If someone
wants to take responsibility for back-porting fixes to the 4.7 branch,
let me know.

INSTALLATION AND USE

Install the module using the standard process.

Visit Administer > Content > Content Types, edit a content type, and
the vocabularies from the Primary Term for that content type can be
chosen.

Edit a node and select the Primary Term from the drop-down list.

LIMITATIONS

Does not currently work with freetagging vocabularies.
