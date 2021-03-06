/* $Id: ruby_xml_node_set.c,v 1.1.1.1 2003/08/23 15:08:42 xpovolny Exp $ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_node_set.h"

VALUE cXMLNodeSet;

VALUE
ruby_xml_node_set_to_a(VALUE self) {
  int i;
  ruby_xml_node_set *rxnset;
  VALUE nodeobj, set_ary;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);

  if ((void *)rxnset->node_set == NULL || (void *)rxnset->node_set->nodeNr == NULL)
    return(Qnil);

  set_ary = rb_ary_new();
  for (i = 0; i < rxnset->node_set->nodeNr; i++) {
    nodeobj = ruby_xml_node_new2(cXMLNode, rxnset->xd, rxnset->node_set->nodeTab[i]);
    rb_ary_push(set_ary, nodeobj);
  }
  return(set_ary);
}


VALUE
ruby_xml_node_set_each(VALUE self) {
  int i;
  ruby_xml_node_set *rxnset;
  VALUE nodeobj;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);

  if (rxnset->node_set == NULL)
    return(Qnil);

  for (i = 0; i < rxnset->node_set->nodeNr; i++) {
    switch(rxnset->node_set->nodeTab[i]->type) {
    case XML_ATTRIBUTE_NODE:
      nodeobj = ruby_xml_attr_new2(cXMLAttr, rxnset->xd, (xmlAttrPtr)rxnset->node_set->nodeTab[i]);
      break;
    default:
      nodeobj = ruby_xml_node_new2(cXMLNode, rxnset->xd, rxnset->node_set->nodeTab[i]);
    }

    rb_yield(nodeobj);
  }
  return(self);
}


void
ruby_xml_node_set_free(ruby_xml_node_set *rxnset) {
  rx_xpath_data *data;

  switch(rxnset->data_type) {
  case RUBY_LIBXML_SRC_TYPE_NULL:
    break;
  case RUBY_LIBXML_SRC_TYPE_XPATH:
    data = rxnset->data;
    free((rx_xpath_data *)data);
  default:
    rb_fatal("Unknown data type, %d", rxnset->data_type);
  }

  /* Don't need to free the node set because the nodeset is a child of
     the XPath object that created the set.
    if (rxnset->node_set != NULL)
      xmlXPathFreeNodeSet(rxnset->node_set);
  */

  free(rxnset);
}


VALUE
ruby_xml_node_set_length(VALUE self) {
  ruby_xml_node_set *rxnset;
  Data_Get_Struct(self, ruby_xml_node_set, rxnset);
  if (rxnset->node_set == NULL)
    return(Qnil);
  else
    return(INT2NUM(rxnset->node_set->nodeNr));
}


static void
ruby_xml_node_set_mark(ruby_xml_node_set *rxnset) {
  if (rxnset == NULL) return;
  if (!NIL_P(rxnset->xd)) rb_gc_mark(rxnset->xd);
  if (!NIL_P(rxnset->xpath)) rb_gc_mark(rxnset->xpath);
}


VALUE
ruby_xml_node_set_new(VALUE class, VALUE xd, VALUE xpath,
			    xmlNodeSetPtr node_set) {
  ruby_xml_node_set *rxnset;

  rxnset = ALLOC(ruby_xml_node_set);
  rxnset->node_set = node_set;
  rxnset->data = NULL;
  rxnset->data_type = RUBY_LIBXML_SRC_TYPE_NULL;
  rxnset->xd = xd;
  rxnset->xpath = xpath;
  return(Data_Wrap_Struct(class, ruby_xml_node_set_mark,
			  ruby_xml_node_set_free, rxnset));
}


VALUE
ruby_xml_node_set_new2(VALUE xd, VALUE xpath,
			     xmlNodeSetPtr node_set) {
  return(ruby_xml_node_set_new(cXMLNodeSet, xd, xpath, node_set));
}


VALUE
ruby_xml_node_set_xpath_get(VALUE self) {
  ruby_xml_node_set *rxnset;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);
  if (NIL_P(rxnset->xpath))
    return(Qnil);
  else
    return(rxnset->xpath);
}


VALUE
ruby_xml_node_set_xpath_data_get(VALUE self) {
  ruby_xml_node_set *rxnset;
  rx_xpath_data *rxxpd;

  Data_Get_Struct(self, ruby_xml_node_set, rxnset);
  if (rxnset->data_type != RUBY_LIBXML_SRC_TYPE_XPATH)
    return(Qnil);

  rxxpd = (rx_xpath_data *)rxnset->data;
  return(rxxpd->ctxt);
}


void
ruby_init_xml_node_set(void) {
  cXMLNodeSet = rb_define_class_under(cXMLNode, "Set", rb_cObject);

  rb_define_method(cXMLNodeSet, "to_a", ruby_xml_node_set_to_a, 0);
  rb_define_method(cXMLNodeSet, "each", ruby_xml_node_set_each, 0);
  rb_define_method(cXMLNodeSet, "length", ruby_xml_node_set_length, 0);
  rb_define_method(cXMLNodeSet, "xpath", ruby_xml_node_set_xpath_get, 0);
  rb_define_method(cXMLNodeSet, "xpath_ctxt",
		   ruby_xml_node_set_xpath_data_get, 0);
}
