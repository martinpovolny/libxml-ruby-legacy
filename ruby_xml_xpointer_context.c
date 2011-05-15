/* $Id: ruby_xml_xpointer_context.c,v 1.1.1.1 2003/08/23 15:08:42 xpovolny Exp $ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_xpointer_context.h"

VALUE cXMLXPointerContext;
VALUE eXMLXPointehContextInvalidPath;
VALUE eXMLXPointerContextInvalidPath;

void
ruby_init_xml_xpointer_context(void) {
  cXMLXPointerContext = rb_define_class_under(cXMLXPointer, "Context", cXMLXPathContext);
  eXMLXPointerContextInvalidPath = rb_define_class_under(cXMLXPointerContext, "InvalidPath", rb_eException);
}
