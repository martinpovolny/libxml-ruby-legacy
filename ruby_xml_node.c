/* $Id: ruby_xml_node.c,v 1.4 2004/04/14 19:20:26 xpovolny Exp $ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_node.h"

VALUE cXMLNode;
VALUE eXMLNodeSetNamespace;
VALUE eXMLNodeFailedModify;
VALUE eXMLNodeUnknownType;

VALUE
ruby_xml_node_attribute_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ATTRIBUTE_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_attribute_decl_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ATTRIBUTE_DECL)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_base_get(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->doc == NULL)
    return(Qnil);

  return(rb_str_new2((char*)xmlNodeGetBase(rxn->node->doc, rxn->node)));
}


VALUE
ruby_xml_node_base_set(VALUE self, VALUE uri) {
  ruby_xml_node *node;

  Check_Type(uri, T_STRING);
  Data_Get_Struct(self, ruby_xml_node, node);
  if (node->node->doc == NULL)
    return(Qnil);

  xmlNodeSetBase(node->node, (unsigned char*)STR2CSTR(uri));
  return(Qtrue);
}


VALUE
ruby_xml_node_cdata_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_CDATA_SECTION_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_comment_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_COMMENT_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_content_add(VALUE self, VALUE obj) {
  ruby_xml_node *node;
  VALUE str;

  Data_Get_Struct(self, ruby_xml_node, node);
  if (rb_obj_is_kind_of(obj, cXMLNode)) {
    return(ruby_xml_node_child_set(self, obj));
  } else if (TYPE(obj) == T_STRING) {
    xmlNodeAddContent(node->node, (unsigned char*)STR2CSTR(obj));
    return(obj);
  } else {
    str = rb_obj_as_string(obj);
    if (NIL_P(str) || TYPE(str) != T_STRING)
      rb_raise(rb_eTypeError, "invalid argumnt: must be string or XML::Node");

    xmlNodeAddContent(node->node, (unsigned char*)STR2CSTR(str));
    return(obj);
  }
}


VALUE
ruby_xml_node_content_get(VALUE self) {
  ruby_xml_node *rxn;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ELEMENT_NODE || rxn->node->content == NULL)
    return(Qnil);
  else {
    return(rb_str_new2((char *)rxn->node->content));
  }
}


VALUE
ruby_xml_node_content_set(VALUE self, VALUE content) {
  ruby_xml_node *node;

  Check_Type(content, T_STRING);
  Data_Get_Struct(self, ruby_xml_node, node);
  xmlNodeSetContent(node->node, (unsigned char*)STR2CSTR(content));
  return(Qtrue);
}


VALUE
ruby_xml_node_content_stripped_get(VALUE self) {
  ruby_xml_node *rxn;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->content == NULL)
    return(Qnil);
  else
    return(rb_str_new2((char *)xmlNodeGetContent(rxn->node)));
}


VALUE
ruby_xml_node_child_get(VALUE self) {
  ruby_xml_node *node;
  xmlNodePtr tmp;

  Data_Get_Struct(self, ruby_xml_node, node);

  switch (node->node->type) {
  case XML_ELEMENT_NODE:
  case XML_ENTITY_REF_NODE:
  case XML_ENTITY_NODE:
  case XML_PI_NODE:
  case XML_COMMENT_NODE:
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
  case XML_DTD_NODE:
    tmp = node->node->children;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) node->node;
      tmp = attr->children;
      break;
    }
  default:
    tmp = NULL;
    break;
  }

  if (tmp == NULL)
    return(Qnil);
  else
    return(ruby_xml_node_new2(cXMLNode, node->xd, tmp));
}


VALUE
ruby_xml_node_child_q(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  node = NULL;
  switch (rxn->node->type) {
  case XML_ELEMENT_NODE:
  case XML_ENTITY_REF_NODE:
  case XML_ENTITY_NODE:
  case XML_PI_NODE:
  case XML_COMMENT_NODE:
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
  case XML_DTD_NODE:
    node = rxn->node->children;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = attr->children;
      break;
    }
  default:
    node = NULL;
  }

  if (node == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


VALUE
ruby_xml_node_child_set(VALUE self, VALUE rnode) {
  ruby_xml_node *cnode, *pnode;
  xmlNodePtr ret;

  if (rb_obj_is_kind_of(rnode, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "Must pass an XML::Node object");

  Data_Get_Struct(self,  ruby_xml_node, pnode);
  Data_Get_Struct(rnode, ruby_xml_node, cnode);

  ret = xmlAddChild(pnode->node, cnode->node);
  if (ret == NULL)
    rb_raise(eXMLNodeFailedModify, "unable to add a child to the document");

  ruby_xml_node_set_ptr(rnode, 1);
  return(ruby_xml_node_new2(cXMLNode, pnode->xd, ret));
}


VALUE
ruby_xml_node_doc(VALUE self) {
  ruby_xml_document *rxd;
  ruby_xml_node *rxn;
  xmlDocPtr doc;
  VALUE docobj;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
    doc = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      doc = attr->doc;
      break;
    }
  case XML_NAMESPACE_DECL:
    doc = NULL;
    break;
  default:
    doc = rxn->node->doc;
    break;
  }

  if (doc == NULL)
    return(Qnil);

  docobj = ruby_xml_document_new(cXMLDocument, doc);
  Data_Get_Struct(docobj, ruby_xml_document, rxd);
  rxd->is_ptr = 1;
  return(docobj);
}


VALUE
ruby_xml_node_docbook_doc_q(VALUE self) {
#ifdef LIBXML_DOCB_ENABLED
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_DOCB_DOCUMENT_NODE)
    return(Qtrue);
  else
    return(Qfalse);
#else
  rb_warn("libxml compiled without docbook support");
  return(Qfalse);
#endif
}


VALUE
ruby_xml_node_doctype_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_DOCUMENT_TYPE_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_document_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_DOCUMENT_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_dtd_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_DTD_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_dump(VALUE self) {
  ruby_xml_node *rxn;
  xmlBufferPtr buf;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  if (rxn->node->doc == NULL)
    return(Qnil);

  buf = xmlBufferCreate();
  xmlNodeDump(buf, rxn->node->doc, rxn->node, 0, 1);
  xmlBufferDump(stdout, buf);
  xmlBufferFree(buf);
  return(Qtrue);
}


VALUE
ruby_xml_node_debug_dump(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  if (rxn->node->doc == NULL)
    return(Qnil);

  xmlElemDump(stdout, rxn->node->doc, rxn->node);
  return(Qtrue);
}


VALUE
ruby_xml_node_element_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ELEMENT_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_element_decl_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ELEMENT_DECL)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_empty_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node == NULL)
    return(Qnil);

  return((xmlIsBlankNode(rxn->node) == 1) ? Qtrue : Qfalse);
}


VALUE
ruby_xml_node_entity_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ENTITY_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_entity_ref_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ENTITY_REF_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_find(int argc, VALUE *argv, VALUE self) {
  int i, vargc;
  VALUE *vargv;

  if (argc > 2 || argc < 1)
    rb_raise(rb_eArgError, "wrong number of arguments (need 1 or 2)");

  vargc = argc + 1;
  vargv = ALLOC_N(VALUE, vargc + 1);
  vargv[0] = self;
  for (i = 0; i<argc; i++)
    vargv[i + 1] = argv[i];

  return(ruby_xml_xpath_find2(vargc, vargv));
}


VALUE
ruby_xml_node_fragment_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_DOCUMENT_FRAG_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


void ruby_xml_node_free(ruby_xml_node *rxn) {
  if (rxn->node != NULL && !rxn->is_ptr) {
    xmlUnlinkNode(rxn->node);
    xmlFreeNode(rxn->node);
    rxn->node = NULL;
  }

  free(rxn);
}


VALUE
ruby_xml_node_html_doc_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_HTML_DOCUMENT_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_initialize(int argc, VALUE *argv, VALUE class) {
  ruby_xml_node *rxn;
  VALUE name, node, str;

  str = Qnil;

  switch(argc) {
  case 2:
    switch (TYPE(str)) {
    case T_STRING:
      str = argv[1];
      break;
    default:
      str = rb_obj_as_string(argv[1]);
      if (NIL_P(str))
	Check_Type(str, T_STRING);
      break;
    }

    /* Intentionally fall through to case 1: as a way of setting up
     * the object.  Sneaky, but effective.  Probably should use a goto
     * instead. */
  case 1:
    name = argv[0];
    Check_Type(name, T_STRING);
    node = ruby_xml_node_new(class, NULL);
    Data_Get_Struct(node, ruby_xml_node, rxn);
    rxn->node = xmlNewNode(NULL, (unsigned char*)STR2CSTR(name));
    if (rxn->node == NULL)
      return(Qnil);

    if (!NIL_P(str))
      ruby_xml_node_content_set(node, str);

    break;

  default:
    rb_raise(rb_eArgError, "wrong number of arguments (1 or 2)");
  }

  return(node);
}


VALUE
ruby_xml_node_lang_get(VALUE self) {
  ruby_xml_node *rxn;
  unsigned char *lang;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  lang = xmlNodeGetLang(rxn->node);

  if (lang == NULL)
    return(Qnil);
  else
    return(rb_str_new2((char *)lang));
}


VALUE
ruby_xml_node_lang_set(VALUE self, VALUE lang) {
  ruby_xml_node *node;

  Check_Type(lang, T_STRING);
  Data_Get_Struct(self, ruby_xml_node, node);
  xmlNodeSetLang(node->node, (unsigned char*)STR2CSTR(lang));

  return(Qtrue);
}


VALUE
ruby_xml_node_last_get(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_ELEMENT_NODE:
  case XML_ENTITY_REF_NODE:
  case XML_ENTITY_NODE:
  case XML_PI_NODE:
  case XML_COMMENT_NODE:
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
  case XML_DTD_NODE:
    node = rxn->node->last;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = attr->last;
    }
  default:
    node = NULL;
    break;
  }

  if (node == NULL)
    return(Qnil);
  else
    return(ruby_xml_node_new2(cXMLNode, rxn->xd, node));
}


VALUE
ruby_xml_node_last_q(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_ELEMENT_NODE:
  case XML_ENTITY_REF_NODE:
  case XML_ENTITY_NODE:
  case XML_PI_NODE:
  case XML_COMMENT_NODE:
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
  case XML_DTD_NODE:
    node = rxn->node->last;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = attr->last;
    }
  default:
    node = NULL;
    break;
  }

  if (node == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


VALUE
ruby_xml_node_line_num(VALUE self) {
  ruby_xml_node *rxn;
  long line_num;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  if (!xmlLineNumbersDefaultValue)
    rb_warn("Line numbers were not being kept track of: use XML::Parser::default_line_numbers=true");

  line_num = xmlGetLineNo(rxn->node);
  if (line_num == -1)
    return(Qnil);
  else
    return(INT2NUM((long)line_num));
}


VALUE
ruby_xml_node_xlink_q(VALUE self) {
  ruby_xml_node *node;
  ruby_xml_document *doc;
  xlinkType xlt;

  Data_Get_Struct(self, ruby_xml_node, node);
  Data_Get_Struct(node->xd, ruby_xml_document, doc);
  xlt = xlinkIsLink(doc->doc, node->node);

  if (xlt == XLINK_TYPE_NONE)
    return(Qfalse);
  else
    return(Qtrue);
}


VALUE
ruby_xml_node_xlink_type(VALUE self) {
  ruby_xml_node *node;
  ruby_xml_document *doc;
  xlinkType xlt;

  Data_Get_Struct(self, ruby_xml_node, node);
  Data_Get_Struct(node->xd, ruby_xml_document, doc);
  xlt = xlinkIsLink(doc->doc, node->node);

  if (xlt == XLINK_TYPE_NONE)
    return(Qnil);
  else
    return(INT2NUM(xlt));
}


VALUE
ruby_xml_node_xlink_type_name(VALUE self) {
  ruby_xml_node *node;
  ruby_xml_document *doc;
  xlinkType xlt;

  Data_Get_Struct(self, ruby_xml_node, node);
  Data_Get_Struct(node->xd, ruby_xml_document, doc);
  xlt = xlinkIsLink(doc->doc, node->node);

  switch(xlt) {
  case XLINK_TYPE_NONE:
    return(Qnil);
  case XLINK_TYPE_SIMPLE:
    return(rb_str_new2("simple"));
  case XLINK_TYPE_EXTENDED:
    return(rb_str_new2("extended"));
  case XLINK_TYPE_EXTENDED_SET:
    return(rb_str_new2("extended_set"));
  default:
    rb_fatal("Unknowng xlink type, %d", xlt);
  }
}


static void
ruby_xml_node_mark(ruby_xml_node *rxn) {
  if (rxn == NULL) return;
  if (!NIL_P(rxn->xd)) rb_gc_mark(rxn->xd);
}


VALUE
ruby_xml_node_name_get(VALUE self) {
  ruby_xml_node *rxn;
  const xmlChar *name;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
    {
      xmlDocPtr doc = (xmlDocPtr) rxn->node;
      name = doc->URL;
      break;
    }
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      name = attr->name;
      break;
    }
  case XML_NAMESPACE_DECL:
    {
      xmlNsPtr ns = (xmlNsPtr) rxn->node;
      name = ns->prefix;
      break;
    }
  default:
    name = rxn->node->name;
    break;
  }

  if (rxn->node->name == NULL)
    return(Qnil);
  else
    return(rb_str_new2((char *)name));
}


VALUE
ruby_xml_node_name_set(VALUE self, VALUE name) {
  ruby_xml_node *node;

  Check_Type(name, T_STRING);
  Data_Get_Struct(self, ruby_xml_node, node);
  xmlNodeSetName(node->node, (unsigned char *)STR2CSTR(name));
  return(Qtrue);
}


VALUE
ruby_xml_node_namespace_get(VALUE self) {
  ruby_xml_node *node;
  xmlNsPtr *nsList, *cur;
  VALUE arr, ns;

  Data_Get_Struct(self, ruby_xml_node, node);
  if (node->node == NULL)
    return(Qnil);

  nsList = xmlGetNsList(node->node->doc, node->node);

  if (nsList == NULL)
    return(Qnil);

  arr = rb_ary_new();
  for (cur = nsList; *cur != NULL; cur++) {
    ns = ruby_xml_ns_new2(cXMLNS, node->xd, *cur);
    if (ns == Qnil)
      continue;
    else
      rb_ary_push(arr, ns);
  }
  xmlFree(nsList);

  return(arr);
}


VALUE
ruby_xml_node_namespace_get_node(VALUE self) {
  ruby_xml_node *node;

  Data_Get_Struct(self, ruby_xml_node, node);
  if (node->node->ns == NULL)
    return(Qnil);
  else
    return(ruby_xml_ns_new2(cXMLNS, node->xd, node->node->ns));
}


VALUE
ruby_xml_node_namespace_set(int argc, VALUE *argv, VALUE self) {
  VALUE rns, rprefix;
  ruby_xml_node *rxn;
  ruby_xml_ns *rxns;
  xmlNsPtr ns;
  char *cp, *href;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  switch (argc) {
  case 1:
    rns = argv[0];
    if (TYPE(rns) == T_STRING) {
      cp = strchr(STR2CSTR(rns), (int)':');
      if (cp == NULL) {
	rprefix = rns;
	href = NULL;
      } else {
	rprefix = rb_str_new(STR2CSTR(rns), (int)((long)cp - (long)STR2CSTR(rns)));
	href = &cp[1]; /* skip the : */
      }
    } else if (rb_obj_is_kind_of(rns, cXMLNS) == Qtrue) {
      Data_Get_Struct(self, ruby_xml_ns, rxns);
      xmlSetNs(rxn->node, rxns->ns);
      return(rns);
    } else
      rb_raise(rb_eTypeError, "must pass a string or an XML::Ns object");

    /* Fall through to next case because when argc == 1, we need to
     * manually setup the additional args unless the arg passed is of
     * cXMLNS type */
  case 2:
    /* Don't want this code run in the fall through case */
    if (argc == 2) {
      rprefix = argv[0];
      href = STR2CSTR(argv[1]);
    }

    ns = xmlNewNs(rxn->node, (unsigned char *)href, (unsigned char *)STR2CSTR(rprefix));
    if (ns == NULL)
      rb_raise(eXMLNodeSetNamespace, "unable to set the namespace");
    else
      return(ruby_xml_ns_new2(cXMLNS, rxn->xd, ns));
    break;

  default:
    rb_raise(rb_eArgError, "wrong number of arguments (1 or 2)");
  }

  /* can't get here */
  return(Qnil);
}


VALUE
ruby_xml_node_namespace_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_NAMESPACE_DECL)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_new(VALUE class, xmlNodePtr node) {
  ruby_xml_node *rxn;

  rxn = ALLOC(ruby_xml_node);
  rxn->is_ptr = 0;
  rxn->node = node;
  rxn->xd = Qnil;
  return(Data_Wrap_Struct(class, ruby_xml_node_mark,
			  ruby_xml_node_free, rxn));
}


VALUE
ruby_xml_node_new2(VALUE class, VALUE xd, xmlNodePtr node) {
  ruby_xml_node *rxn;

  rxn = ALLOC(ruby_xml_node);
  rxn->is_ptr = 1;
  rxn->node = node;
  if (NIL_P(xd))
    rxn->xd = Qnil;
  else
    rxn->xd = xd;
  return(Data_Wrap_Struct(class, ruby_xml_node_mark,
			  ruby_xml_node_free, rxn));
}


VALUE
ruby_xml_node_next_get(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
    node = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = (xmlNodePtr) attr->next;
      break;
    }
  case XML_NAMESPACE_DECL:
    {
      xmlNsPtr ns = (xmlNsPtr) rxn->node;
      node = (xmlNodePtr) ns->next;
      break;
    }
  default:
    node = rxn->node->next;
    break;
  }

  if (node == NULL)
    return(Qnil);
  else
    return(ruby_xml_node_new2(cXMLNode, rxn->xd, node));
}


VALUE
ruby_xml_node_next_q(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
    node = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = (xmlNodePtr) attr->next;
      break;
    }
  case XML_NAMESPACE_DECL:
    {
      xmlNsPtr ns = (xmlNsPtr) rxn->node;
      node = (xmlNodePtr) ns->next;
      break;
    }
  default:
    node = rxn->node->next;
    break;
  }

  if (node == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


VALUE
ruby_xml_node_notation_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_NOTATION_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_ns_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->ns == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


VALUE
ruby_xml_node_ns_def_get(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->nsDef == NULL)
    return(Qnil);
  else
    return(ruby_xml_ns_new2(cXMLNS, rxn->xd, rxn->node->nsDef));
}


VALUE
ruby_xml_node_ns_def_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->nsDef == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


VALUE
ruby_xml_node_parent_get(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
  case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
    node = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = attr->parent;
    }
  case XML_ENTITY_DECL:
  case XML_NAMESPACE_DECL:
  case XML_XINCLUDE_START:
  case XML_XINCLUDE_END:
    node = NULL;
    break;
  default:
    node = rxn->node->parent;
    break;
  }

  if (node == NULL)
    return(Qnil);
  else
    return(ruby_xml_node_new2(cXMLNode, rxn->xd, node));
}


VALUE
ruby_xml_node_parent_q(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
  case XML_HTML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
    node = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = attr->parent;
    }
  case XML_ENTITY_DECL:
  case XML_NAMESPACE_DECL:
  case XML_XINCLUDE_START:
  case XML_XINCLUDE_END:
    node = NULL;
    break;
  default:
    node = rxn->node->parent;
    break;
  }

  if (node == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


VALUE
ruby_xml_node_path(VALUE self) {
  ruby_xml_node *rxn;
  unsigned char *path;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  path = xmlGetNodePath(rxn->node);

  if (path == NULL)
    return(Qnil);
  else
    return(rb_str_new2((char *)path));
}


VALUE
ruby_xml_node_pi_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_PI_NODE)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_pointer(VALUE self, VALUE xptr_str) {
  return(ruby_xml_xpointer_point2(self, xptr_str));
}


VALUE
ruby_xml_node_prev_get(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
  case XML_NAMESPACE_DECL:
    node = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = (xmlNodePtr) attr->next;
    }
    break;
  default:
    node = rxn->node->next;
    break;
  }

  if (node == NULL)
    return(Qnil);
  else
    return(ruby_xml_node_new2(cXMLNode, rxn->xd, node));
}


VALUE
ruby_xml_node_prev_q(VALUE self) {
  ruby_xml_node *rxn;
  xmlNodePtr node;

  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch (rxn->node->type) {
  case XML_DOCUMENT_NODE:
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
#endif
  case XML_HTML_DOCUMENT_NODE:
  case XML_NAMESPACE_DECL:
    node = NULL;
    break;
  case XML_ATTRIBUTE_NODE:
    {
      xmlAttrPtr attr = (xmlAttrPtr) rxn->node;
      node = (xmlNodePtr) attr->next;
    }
    break;
  default:
    node = rxn->node->next;
    break;
  }

  if (node == NULL)
    return(Qfalse);
  else
    return(Qtrue);
}


VALUE
ruby_xml_node_property_get(VALUE self, VALUE prop) {
  ruby_xml_node *rxn;
  unsigned char *p;

  Check_Type(prop, T_STRING);

  Data_Get_Struct(self, ruby_xml_node, rxn);
  p = xmlGetProp(rxn->node, (unsigned char*)STR2CSTR(prop));

  if (p == NULL)
    return(Qnil);
  else
    return(rb_str_new2((char *)p));
}


VALUE
ruby_xml_node_property_set(VALUE self, VALUE key, VALUE val) {
  ruby_xml_node *node;
  ruby_xml_attr *rxa;
  xmlAttrPtr attr;
  VALUE rattr;

  Check_Type(key, T_STRING);
  Check_Type(val, T_STRING);
  Data_Get_Struct(self, ruby_xml_node, node);

  attr = xmlSetProp(node->node, (unsigned char)STR2CSTR(key), (unsigned char)STR2CSTR(val));
  if (attr == NULL) {
    attr = xmlNewProp(node->node, (unsigned char)STR2CSTR(key), (unsigned char)STR2CSTR(val));
    if (attr == NULL)
      return(Qnil);
  }
  rattr = ruby_xml_attr_new(cXMLAttr, node->xd, attr);
  Data_Get_Struct(rattr, ruby_xml_attr, rxa);
  rxa->is_ptr = 1;
  return(rattr);
}


VALUE
ruby_xml_node_properties_get(VALUE self) {
  ruby_xml_node *node;
  xmlAttrPtr attr;

  Data_Get_Struct(self, ruby_xml_node, node);

  if (node->node->type == XML_ELEMENT_NODE) {
    attr = node->node->properties;
    return(ruby_xml_attr_new(cXMLAttr, node->xd, attr));
  } else {
    return(Qnil);
  }
}


VALUE
ruby_xml_node_properties_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_ELEMENT_NODE && rxn->node->properties != NULL)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_search_href(VALUE self, VALUE href) {
  ruby_xml_document *doc;
  ruby_xml_node *node;

  Check_Type(href, T_STRING);
  Data_Get_Struct(self, ruby_xml_node, node);
  Data_Get_Struct(node->xd, ruby_xml_document, doc);
  return(ruby_xml_ns_new2(cXMLNS, node->xd,
			  xmlSearchNsByHref(doc->doc, node->node,
					    STR2CSTR(href))));
}


VALUE
ruby_xml_node_search_ns(VALUE self, VALUE ns) {
  ruby_xml_document *doc;
  ruby_xml_node *node;

  Check_Type(ns, T_STRING);
  Data_Get_Struct(self, ruby_xml_node, node);
  Data_Get_Struct(node->xd, ruby_xml_document, doc);
  return(ruby_xml_ns_new2(cXMLNS, node->xd,
			  xmlSearchNs(doc->doc, node->node,
					    STR2CSTR(ns))));
}


VALUE
ruby_xml_node_set_ptr(VALUE node, int is_ptr) {
  ruby_xml_node *rxn;
  Data_Get_Struct(node, ruby_xml_node, rxn);
  rxn->is_ptr = is_ptr;
  return(Qtrue);
}


VALUE
ruby_xml_node_sibling_set(VALUE self, VALUE rnode) {
  ruby_xml_node *cnode, *pnode;
  xmlNodePtr ret;

  if (rb_obj_is_kind_of(rnode, cXMLNode) == Qfalse)
    rb_raise(rb_eTypeError, "Must pass an XML::Node object");

  Data_Get_Struct(self,  ruby_xml_node, pnode);
  Data_Get_Struct(rnode, ruby_xml_node, cnode);

  ret = xmlAddSibling(pnode->node, cnode->node);
  if (ret == NULL)
    rb_raise(eXMLNodeFailedModify, "unable to add a sibling to the document");

  cnode->is_ptr = 1;
  return(ruby_xml_node_new2(cXMLNode, pnode->xd, ret));
}


VALUE
ruby_xml_node_space_preserve_get(VALUE self) {
  ruby_xml_node *rxn;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  return(INT2NUM(xmlNodeGetSpacePreserve(rxn->node)));
}


VALUE
ruby_xml_node_space_preserve_set(VALUE self, VALUE bool) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  if (TYPE(bool) == T_FALSE)
    xmlNodeSetSpacePreserve(rxn->node, 1);
  else
    xmlNodeSetSpacePreserve(rxn->node, 0);

  return(Qnil);
}


VALUE
ruby_xml_node_text_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node == NULL)
    return(Qnil);

  return((xmlNodeIsText(rxn->node) == 1) ? Qtrue : Qfalse);
}


VALUE
ruby_xml_node_to_s(VALUE self) {
  ruby_xml_node *rxn;
  xmlBufferPtr buf;
  VALUE result;

  Data_Get_Struct(self, ruby_xml_node, rxn);
  buf = xmlBufferCreate();
  xmlNodeDump(buf, rxn->node->doc, rxn->node, 0, 1);
  result = rb_str_new2(buf->content);
  
  xmlBufferFree(buf);
  return result;
}


VALUE
ruby_xml_node_type(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  return(INT2NUM(rxn->node->type));
}


VALUE
ruby_xml_node_type_name(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);

  switch(rxn->node->type) {
  case XML_ELEMENT_NODE:
    return(rb_str_new2("element"));
  case XML_ATTRIBUTE_NODE:
    return(rb_str_new2("attribute"));
  case XML_TEXT_NODE:
    return(rb_str_new2("text"));
  case XML_CDATA_SECTION_NODE:
    return(rb_str_new2("cdata"));
  case XML_ENTITY_REF_NODE:
    return(rb_str_new2("entity_ref"));
  case XML_ENTITY_NODE:
    return(rb_str_new2("entity"));
  case XML_PI_NODE:
    return(rb_str_new2("pi"));
  case XML_COMMENT_NODE:
    return(rb_str_new2("comment"));
  case XML_DOCUMENT_NODE:
    return(rb_str_new2("document_xml"));
  case XML_DOCUMENT_TYPE_NODE:
    return(rb_str_new2("doctype"));
  case XML_DOCUMENT_FRAG_NODE:
    return(rb_str_new2("fragment"));
  case XML_NOTATION_NODE:
    return(rb_str_new2("notation"));
  case XML_HTML_DOCUMENT_NODE:
    return(rb_str_new2("document_html"));
  case XML_DTD_NODE:
    return(rb_str_new2("dtd"));
  case XML_ELEMENT_DECL:
    return(rb_str_new2("elem_decl"));
  case XML_ATTRIBUTE_DECL:
    return(rb_str_new2("attribute_decl"));
  case XML_ENTITY_DECL:
    return(rb_str_new2("entity_decl"));
  case XML_NAMESPACE_DECL:
    return(rb_str_new2("namespace"));
  case XML_XINCLUDE_START:
    return(rb_str_new2("xinclude_start"));
  case XML_XINCLUDE_END:
    return(rb_str_new2("xinclude_end"));
#ifdef LIBXML_DOCB_ENABLED
  case XML_DOCB_DOCUMENT_NODE:
    return(rb_str_new2("document_docbook"));
#endif
  default:
    rb_raise(eXMLNodeUnknownType, "Unknown node type: %n", rxn->node->type);
    return(Qfalse);
  }
}


VALUE
ruby_xml_node_xinclude_end_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_XINCLUDE_END)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_node_xinclude_start_q(VALUE self) {
  ruby_xml_node *rxn;
  Data_Get_Struct(self, ruby_xml_node, rxn);
  if (rxn->node->type == XML_XINCLUDE_START)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE /* MUFF */
ruby_xml_node_copy(VALUE self, VALUE deep) {
  ruby_xml_node *rxn, *n_rxn;
  VALUE n_node;
  
  Data_Get_Struct(self, ruby_xml_node, rxn);

  n_node = ruby_xml_node_new(cXMLNode, NULL); // class??
  Data_Get_Struct(n_node, ruby_xml_node, n_rxn);

  n_rxn->node = xmlCopyNode( rxn->node, ((deep==Qnil)||(deep==Qfalse))?0:1 );
  if (rxn->node == NULL)
    return(Qnil);

  return n_node;
}


void
ruby_init_xml_node(void) {
  cXMLNode = rb_define_class_under(mXML, "Node", rb_cObject);
  eXMLNodeSetNamespace = rb_define_class_under(cXMLNode, "SetNamespace", rb_eException);
  eXMLNodeFailedModify = rb_define_class_under(cXMLNode, "FailedModify", rb_eException);
  eXMLNodeUnknownType = rb_define_class_under(cXMLNode, "UnknownType", rb_eException);

  rb_define_const(cXMLNode, "SPACE_DEFAULT", INT2NUM(0));
  rb_define_const(cXMLNode, "SPACE_PRESERVE", INT2NUM(1));
  rb_define_const(cXMLNode, "SPACE_NOT_INHERIT", INT2NUM(-1));
  rb_define_const(cXMLNode, "XLINK_ACTUATE_AUTO", INT2NUM(1));
  rb_define_const(cXMLNode, "XLINK_ACTUATE_NONE", INT2NUM(0));
  rb_define_const(cXMLNode, "XLINK_ACTUATE_ONREQUEST", INT2NUM(2));
  rb_define_const(cXMLNode, "XLINK_SHOW_EMBED", INT2NUM(2));
  rb_define_const(cXMLNode, "XLINK_SHOW_NEW", INT2NUM(1));
  rb_define_const(cXMLNode, "XLINK_SHOW_NONE", INT2NUM(0));
  rb_define_const(cXMLNode, "XLINK_SHOW_REPLACE", INT2NUM(3));
  rb_define_const(cXMLNode, "XLINK_TYPE_EXTENDED", INT2NUM(2));
  rb_define_const(cXMLNode, "XLINK_TYPE_EXTENDED_SET", INT2NUM(3));
  rb_define_const(cXMLNode, "XLINK_TYPE_NONE", INT2NUM(0));
  rb_define_const(cXMLNode, "XLINK_TYPE_SIMPLE", INT2NUM(1));

  rb_define_singleton_method(cXMLNode, "new", ruby_xml_node_initialize, -1);

  rb_define_method(cXMLNode, "<<", ruby_xml_node_content_add, 1);
  rb_define_method(cXMLNode, "[]", ruby_xml_node_property_get, 1);
  rb_define_method(cXMLNode, "[]=", ruby_xml_node_property_set, 2);
  rb_define_method(cXMLNode, "attribute?", ruby_xml_node_attribute_q, 0);
  rb_define_method(cXMLNode, "attribute_decl?", ruby_xml_node_attribute_decl_q, 0);
  rb_define_method(cXMLNode, "base", ruby_xml_node_base_get, 0);
  rb_define_method(cXMLNode, "base=", ruby_xml_node_base_set, 1);
  rb_define_method(cXMLNode, "blank?", ruby_xml_node_empty_q, 0);
  rb_define_method(cXMLNode, "cdata?", ruby_xml_node_cdata_q, 0);
  rb_define_method(cXMLNode, "comment?", ruby_xml_node_comment_q, 0);
  rb_define_method(cXMLNode, "copy", ruby_xml_node_copy, 1);
  rb_define_method(cXMLNode, "child", ruby_xml_node_child_get, 0);
  rb_define_method(cXMLNode, "child?", ruby_xml_node_child_q, 0);
  rb_define_method(cXMLNode, "child=", ruby_xml_node_child_set, 1);
  rb_define_method(cXMLNode, "children", ruby_xml_node_child_get, 0);
  rb_define_method(cXMLNode, "children?", ruby_xml_node_child_q, 0);
  rb_define_method(cXMLNode, "content", ruby_xml_node_content_get, 0);
  rb_define_method(cXMLNode, "content=", ruby_xml_node_content_set, 1);
  rb_define_method(cXMLNode, "content_stripped", ruby_xml_node_content_stripped_get, 0);
  rb_define_method(cXMLNode, "doc", ruby_xml_node_doc, 0);
  rb_define_method(cXMLNode, "docbook_doc?", ruby_xml_node_docbook_doc_q, 0);
  rb_define_method(cXMLNode, "doctype?", ruby_xml_node_doctype_q, 0);
  rb_define_method(cXMLNode, "document?", ruby_xml_node_document_q, 0);
  rb_define_method(cXMLNode, "dtd?", ruby_xml_node_dtd_q, 0);
  rb_define_method(cXMLNode, "dump", ruby_xml_node_dump, 0);
  rb_define_method(cXMLNode, "debug_dump", ruby_xml_node_debug_dump, 0);
  rb_define_method(cXMLNode, "element?", ruby_xml_node_element_q, 0);
  rb_define_method(cXMLNode, "element_decl?", ruby_xml_node_element_decl_q, 0);
  rb_define_method(cXMLNode, "empty?", ruby_xml_node_empty_q, 0);
  rb_define_method(cXMLNode, "entity?", ruby_xml_node_entity_q, 0);
  rb_define_method(cXMLNode, "entity_ref?", ruby_xml_node_entity_ref_q, 0);
  rb_define_method(cXMLNode, "find", ruby_xml_node_find, -1);
  rb_define_method(cXMLNode, "fragment?", ruby_xml_node_fragment_q, 0);
  rb_define_method(cXMLNode, "html_doc?", ruby_xml_node_html_doc_q, 0);
  rb_define_method(cXMLNode, "lang", ruby_xml_node_lang_get, 0);
  rb_define_method(cXMLNode, "lang=", ruby_xml_node_lang_set, 1);
  rb_define_method(cXMLNode, "last", ruby_xml_node_last_get, 0);
  rb_define_method(cXMLNode, "last?", ruby_xml_node_last_q, 0);
  rb_define_method(cXMLNode, "line_num", ruby_xml_node_line_num, 0);
  rb_define_method(cXMLNode, "name", ruby_xml_node_name_get, 0);
  rb_define_method(cXMLNode, "name=", ruby_xml_node_name_set, 1);
  rb_define_method(cXMLNode, "namespace", ruby_xml_node_namespace_get, 0);
  rb_define_method(cXMLNode, "namespace_node", ruby_xml_node_namespace_get_node, 0);
  rb_define_method(cXMLNode, "namespace?", ruby_xml_node_namespace_q, 0);
  rb_define_method(cXMLNode, "namespace=", ruby_xml_node_namespace_set, -1);
  rb_define_method(cXMLNode, "next", ruby_xml_node_next_get, 0);
  rb_define_method(cXMLNode, "next?", ruby_xml_node_next_q, 0);
  rb_define_method(cXMLNode, "node_type", ruby_xml_node_type, 0);
  rb_define_method(cXMLNode, "node_type_name", ruby_xml_node_type_name, 0);
  rb_define_method(cXMLNode, "notation?", ruby_xml_node_notation_q, 0);
  rb_define_method(cXMLNode, "ns", ruby_xml_node_namespace_get, 0);
  rb_define_method(cXMLNode, "ns?", ruby_xml_node_ns_q, 0);
  rb_define_method(cXMLNode, "ns_def", ruby_xml_node_ns_def_get, 0);
  rb_define_method(cXMLNode, "ns_def?", ruby_xml_node_ns_def_q, 0);
  rb_define_method(cXMLNode, "parent", ruby_xml_node_parent_get, 0);
  rb_define_method(cXMLNode, "parent?", ruby_xml_node_parent_q, 0);
  rb_define_method(cXMLNode, "path", ruby_xml_node_path, 0);
  rb_define_method(cXMLNode, "pi?", ruby_xml_node_pi_q, 0);
  rb_define_method(cXMLNode, "pointer", ruby_xml_node_pointer, 1);
  rb_define_method(cXMLNode, "prev", ruby_xml_node_prev_get, 0);
  rb_define_method(cXMLNode, "prev?", ruby_xml_node_prev_q, 0);
  rb_define_method(cXMLNode, "property", ruby_xml_node_property_get, 1);
  rb_define_method(cXMLNode, "properties", ruby_xml_node_properties_get, 0);
  rb_define_method(cXMLNode, "properties?", ruby_xml_node_properties_q, 0);
  rb_define_method(cXMLNode, "search_ns", ruby_xml_node_search_ns, 1);
  rb_define_method(cXMLNode, "search_href", ruby_xml_node_search_href, 1);
  rb_define_method(cXMLNode, "sibling=", ruby_xml_node_sibling_set, 1);
  rb_define_method(cXMLNode, "space_preserve", ruby_xml_node_space_preserve_get, 0);
  rb_define_method(cXMLNode, "space_preserve=", ruby_xml_node_space_preserve_set, 1);
  rb_define_method(cXMLNode, "text?", ruby_xml_node_text_q, 0);
  rb_define_method(cXMLNode, "to_s", ruby_xml_node_to_s, 0);
  rb_define_method(cXMLNode, "xinclude_end?", ruby_xml_node_xinclude_end_q, 0);
  rb_define_method(cXMLNode, "xinclude_start?", ruby_xml_node_xinclude_start_q, 0);
  rb_define_method(cXMLNode, "xlink?", ruby_xml_node_xlink_q, 0);
  rb_define_method(cXMLNode, "xlink_type", ruby_xml_node_xlink_type, 0);
  rb_define_method(cXMLNode, "xlink_type_name", ruby_xml_node_xlink_type_name, 0);
}
