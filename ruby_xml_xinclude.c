/* $Id: ruby_xml_xinclude.c,v 1.1.1.1 2003/08/23 15:08:42 xpovolny Exp $ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_xinclude.h"

VALUE cXMLXInclude;
VALUE eXMLXIncludeError;

void
ruby_init_xml_xinclude(void) {
  cXMLXInclude = rb_define_class_under(mXML, "XInclude", rb_cObject);
  eXMLXIncludeError = rb_define_class_under(cXMLXInclude, "Error", rb_eRuntimeError);
}
