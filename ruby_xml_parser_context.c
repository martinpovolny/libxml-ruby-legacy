/* $Id: ruby_xml_parser_context.c,v 1.1.1.1 2003/08/23 15:08:42 xpovolny Exp $ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_parser_context.h"

/* TODO:
 *
 * *) xmlParserInput class/structure
 * *) errNo and mappings
 * *) validity context
 * *) record_info or stats class/structure
 * *) xmlParserNodeInfoSeq
 * *) xmlParserInputState
 */
VALUE cXMLParserContext;

VALUE
ruby_xml_parser_context_data_directory_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->directory == NULL)
    return(Qnil);
  else
    return(rb_str_new2(rxpc->ctxt->directory));
}


VALUE
ruby_xml_parser_context_depth_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  return(INT2NUM(rxpc->ctxt->depth));
}


VALUE
ruby_xml_parser_context_disable_sax_q(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->disableSAX)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_parser_context_doc_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->myDoc == NULL)
    return(Qnil);

  return(ruby_xml_document_new4(cXMLDocument, rxpc->ctxt->myDoc));
}


VALUE
ruby_xml_parser_context_docbook_q(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->html == 2)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_parser_context_encoding_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->encoding == NULL)
    return(Qnil);
  else
    return(rb_str_new2(rxpc->ctxt->encoding));
}


VALUE
ruby_xml_parser_context_errno_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  return(INT2NUM(rxpc->ctxt->errNo));
}


void
ruby_xml_parser_context_free(ruby_xml_parser_context *rxpc) {
  if (rxpc->ctxt != NULL && !rxpc->is_ptr) {
    xmlFreeParserCtxt(rxpc->ctxt);
    ruby_xml_parser_count--;
    rxpc->ctxt = NULL;
  }

  if (ruby_xml_parser_count == 0)
    xmlCleanupParser();

  free(rxpc);
}


VALUE
ruby_xml_parser_context_html_q(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->html == 1)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_parser_context_io_max_num_streams_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  return(INT2NUM(rxpc->ctxt->inputMax));
}


VALUE
ruby_xml_parser_context_io_num_streams_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  return(INT2NUM(rxpc->ctxt->inputNr));
}


VALUE
ruby_xml_parser_context_keep_blanks_q(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->keepBlanks)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_parser_context_name_depth_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  return(INT2NUM(rxpc->ctxt->nameNr));
}


VALUE
ruby_xml_parser_context_name_depth_max_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  return(INT2NUM(rxpc->ctxt->nameMax));
}


VALUE
ruby_xml_parser_context_name_node_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->name == NULL)
    return(Qnil);
  else
    return(rb_str_new2(rxpc->ctxt->name));
}


VALUE
ruby_xml_parser_context_name_tab_get(VALUE self) {
  int i;
  ruby_xml_parser_context *rxpc;
  VALUE tab_ary;

  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->nameTab == NULL)
    return(Qnil);

  tab_ary = rb_ary_new();

  for (i = (rxpc->ctxt->nameNr - 1); i >= 0; i--) {
    if (rxpc->ctxt->nameTab[i] == NULL)
      continue;
    else
      rb_ary_push(tab_ary, rb_str_new2(rxpc->ctxt->nameTab[i]));
  }

  return(tab_ary);
}


VALUE
ruby_xml_parser_context_node_depth_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  return(INT2NUM(rxpc->ctxt->nodeNr));
}


VALUE
ruby_xml_parser_context_node_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->node == NULL)
    return(Qnil);
  else
    return(ruby_xml_node_new2(cXMLNode,
			      ruby_xml_document_new(cXMLDocument, rxpc->ctxt->myDoc),
			      rxpc->ctxt->node));
}


VALUE
ruby_xml_parser_context_node_depth_max_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  return(INT2NUM(rxpc->ctxt->nodeMax));
}


VALUE
ruby_xml_parser_context_num_chars_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  return(LONG2NUM(rxpc->ctxt->nbChars));
}


VALUE
ruby_xml_parser_context_new(VALUE class, xmlParserCtxtPtr ctxt) {
  ruby_xml_parser_context *rxpc;

  rxpc = ALLOC(ruby_xml_parser_context);
  ruby_xml_parser_count++;

  rxpc->ctxt = ctxt;
  rxpc->is_ptr = 0;
  return(Data_Wrap_Struct(class, 0, ruby_xml_parser_context_free, rxpc));
}


VALUE
ruby_xml_parser_context_new2(VALUE class) {
  return(ruby_xml_parser_context_new(class, NULL));
}


VALUE
ruby_xml_parser_context_new3() {
  return(ruby_xml_parser_context_new2(cXMLParserContext));
}


VALUE
ruby_xml_parser_context_replace_entities_q(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->replaceEntities)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_parser_context_replace_entities_set(VALUE self, VALUE bool) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (TYPE(bool) == T_FALSE) {
    rxpc->ctxt->replaceEntities = 0;
    return(Qfalse);
  } else {
    rxpc->ctxt->replaceEntities = 1;
    return(Qfalse);
  }
}


VALUE
ruby_xml_parser_context_space_depth_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  return(INT2NUM(rxpc->ctxt->spaceNr));
}


VALUE
ruby_xml_parser_context_space_depth_max_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  return(INT2NUM(rxpc->ctxt->spaceMax));
}


VALUE
ruby_xml_parser_context_subset_external_q(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->inSubset == 2)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_parser_context_subset_internal_q(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->inSubset == 1)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_parser_context_subset_name_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->intSubName == NULL)
    return(Qnil);
  else
    return(rb_str_new2(rxpc->ctxt->intSubName));
}


VALUE
ruby_xml_parser_context_subset_external_uri_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->extSubURI == NULL)
    return(Qnil);
  else
    return(rb_str_new2(rxpc->ctxt->extSubURI));
}


VALUE
ruby_xml_parser_context_subset_external_system_id_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->extSubSystem == NULL)
    return(Qnil);
  else
    return(rb_str_new2(rxpc->ctxt->extSubSystem));
}


VALUE
ruby_xml_parser_context_standalone_q(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->standalone)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_parser_context_stats_q(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->record_info)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_parser_context_valid_q(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->valid)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_parser_context_validate_q(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->validate)
    return(Qtrue);
  else
    return(Qfalse);
}


VALUE
ruby_xml_parser_context_version_get(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->version == NULL)
    return(Qnil);
  else
    return(rb_str_new2(rxpc->ctxt->version));
}


VALUE
ruby_xml_parser_context_well_formed_q(VALUE self) {
  ruby_xml_parser_context *rxpc;
  Data_Get_Struct(self, ruby_xml_parser_context, rxpc);

  if (rxpc->ctxt->wellFormed)
    return(Qtrue);
  else
    return(Qfalse);
}


void
ruby_init_xml_parser_context(void) {
  cXMLParserContext = rb_define_class_under(cXMLParser, "Context", rb_cObject);

  rb_define_method(cXMLParserContext, "data_directory", ruby_xml_parser_context_data_directory_get, 0);
  rb_define_method(cXMLParserContext, "depth", ruby_xml_parser_context_depth_get, 0);
  rb_define_method(cXMLParserContext, "disable_sax?", ruby_xml_parser_context_disable_sax_q, 0);
  rb_define_method(cXMLParserContext, "doc", ruby_xml_parser_context_doc_get, 0);
  rb_define_method(cXMLParserContext, "docbook?", ruby_xml_parser_context_docbook_q, 0);
  rb_define_method(cXMLParserContext, "encoding", ruby_xml_parser_context_encoding_get, 0);
  rb_define_method(cXMLParserContext, "errno", ruby_xml_parser_context_errno_get, 0);
  rb_define_method(cXMLParserContext, "html?", ruby_xml_parser_context_html_q, 0);
  rb_define_method(cXMLParserContext, "io_max_num_streams", ruby_xml_parser_context_io_max_num_streams_get, 0);
  rb_define_method(cXMLParserContext, "io_num_streams", ruby_xml_parser_context_io_num_streams_get, 0);
  rb_define_method(cXMLParserContext, "keep_blanks?", ruby_xml_parser_context_keep_blanks_q, 0);
  rb_define_method(cXMLParserContext, "name_node", ruby_xml_parser_context_name_node_get, 0);
  rb_define_method(cXMLParserContext, "name_depth", ruby_xml_parser_context_name_depth_get, 0);
  rb_define_method(cXMLParserContext, "name_depth_max", ruby_xml_parser_context_name_depth_max_get, 0);
  rb_define_method(cXMLParserContext, "name_tab", ruby_xml_parser_context_name_tab_get, 0);
  rb_define_method(cXMLParserContext, "node", ruby_xml_parser_context_node_get, 0);
  rb_define_method(cXMLParserContext, "node_depth", ruby_xml_parser_context_node_depth_get, 0);
  rb_define_method(cXMLParserContext, "node_depth_max", ruby_xml_parser_context_node_depth_max_get, 0);
  rb_define_method(cXMLParserContext, "num_chars", ruby_xml_parser_context_num_chars_get, 0);
  rb_define_method(cXMLParserContext, "replace_entities?", ruby_xml_parser_context_replace_entities_q, 0);
  rb_define_method(cXMLParserContext, "replace_entities=", ruby_xml_parser_context_replace_entities_set, 1);
  rb_define_method(cXMLParserContext, "space_depth", ruby_xml_parser_context_space_depth_get, 0);
  rb_define_method(cXMLParserContext, "space_depth_max", ruby_xml_parser_context_space_depth_max_get, 0);
  rb_define_method(cXMLParserContext, "subset_external?", ruby_xml_parser_context_subset_external_q, 0);
  rb_define_method(cXMLParserContext, "subset_external_system_id", ruby_xml_parser_context_subset_external_system_id_get, 0);
  rb_define_method(cXMLParserContext, "subset_external_uri", ruby_xml_parser_context_subset_name_get, 0);
  rb_define_method(cXMLParserContext, "subset_internal?", ruby_xml_parser_context_subset_internal_q, 0);
  rb_define_method(cXMLParserContext, "subset_internal_name", ruby_xml_parser_context_subset_name_get, 0);
  rb_define_method(cXMLParserContext, "stats?", ruby_xml_parser_context_stats_q, 0);
  rb_define_method(cXMLParserContext, "standalone?", ruby_xml_parser_context_standalone_q, 0);
  rb_define_method(cXMLParserContext, "valid", ruby_xml_parser_context_valid_q, 0);
  rb_define_method(cXMLParserContext, "validate?", ruby_xml_parser_context_validate_q, 0);
  rb_define_method(cXMLParserContext, "version", ruby_xml_parser_context_version_get, 0);
  rb_define_method(cXMLParserContext, "well_formed?", ruby_xml_parser_context_well_formed_q, 0);
}
