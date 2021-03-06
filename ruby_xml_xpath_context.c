/* $Id: ruby_xml_xpath_context.c,v 1.1.1.1 2003/08/23 15:08:42 xpovolny Exp $ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_xpath_context.h"


VALUE
ruby_xml_xpath_context_doc_get(VALUE self) {
  ruby_xml_xpath_context *rxxpc;
  Data_Get_Struct(self, ruby_xml_xpath_context, rxxpc);

  return(rxxpc->xd);
}


void
ruby_xml_xpath_context_free(ruby_xml_xpath_context *rxxpc) {
  if (rxxpc->ctxt != NULL) {
    xmlXPathFreeContext(rxxpc->ctxt);
    rxxpc->ctxt = NULL;
  }

  free(rxxpc);
}


void
ruby_xml_xpath_context_mark(ruby_xml_xpath_context *rxxpc) {
  if (rxxpc == NULL) return;
  if (!NIL_P(rxxpc->xd)) rb_gc_mark(rxxpc->xd);
}


VALUE
ruby_xml_xpath_context_new(VALUE class, VALUE xd,
				 xmlXPathContextPtr xxpc) {
  ruby_xml_xpath_context *rxxpc;

  rxxpc = ALLOC(ruby_xml_xpath_context);
  rxxpc->ctxt = xxpc;
  rxxpc->xd = xd;
  return(Data_Wrap_Struct(class, ruby_xml_xpath_context_mark,
			  ruby_xml_xpath_context_free, rxxpc));
}


VALUE
ruby_xml_xpath_context_new2(VALUE xd, xmlXPathContextPtr xxpc) {
  return(ruby_xml_xpath_context_new(cXMLXPathContext, xd, xxpc));
}


VALUE
ruby_xml_xpath_context_new3(VALUE xd) {
  ruby_xml_document *rxd;
  xmlXPathContextPtr ctxt;

  Data_Get_Struct(xd, ruby_xml_document, rxd);
  if (rxd->doc == NULL)
    return(Qnil);

  ctxt = xmlXPathNewContext(rxd->doc);
  if (ctxt == NULL)
    return(Qnil);

  return(ruby_xml_xpath_context_new2(xd, ctxt));
}


VALUE
ruby_xml_xpath_context_new4(VALUE rnode) {
  ruby_xml_node *node;

  Data_Get_Struct(rnode, ruby_xml_node, node);
  return(ruby_xml_xpath_context_new3(node->xd));
}


VALUE
ruby_xml_xpath_context_register_namespace(VALUE self, VALUE prefix, VALUE uri) {
  ruby_xml_xpath_context *rxxpc;

  Data_Get_Struct(self, ruby_xml_xpath_context, rxxpc);
  if (xmlXPathRegisterNs(rxxpc->ctxt, STR2CSTR(prefix), STR2CSTR(uri)) == 0) {
    return(Qtrue);
  } else {
    /* Should raise an exception, IMHO */
    return(Qfalse);
  }
}


void
ruby_init_xml_xpath_context(void) {
  cXMLXPathContext = rb_define_class_under(cXMLXPath, "Context", rb_cObject);

  rb_define_method(cXMLXPathContext, "register_namespace",
		   ruby_xml_xpath_context_register_namespace, 2);
  rb_define_method(cXMLXPathContext, "doc",
		   ruby_xml_xpath_context_doc_get, 0);
}
