/* $Id: ruby_xml_sax_parser.c,v 1.1.1.1 2003/08/23 15:08:42 xpovolny Exp $ */

/* Please see the LICENSE file for copyright and distribution information */

#include "libxml.h"
#include "ruby_xml_sax_parser.h"

VALUE cXMLSaxParser;

void
ruby_xml_sax_parser_free(ruby_xml_sax_parser *rxsp) {
  /* Apparently this isn't needed: time will tell */
  /* if (rxsp->xsh != NULL) */
  /* xmlFreeSax_Parser(rxsp->sax_parser); */
}

VALUE
ruby_xml_sax_parser_new(VALUE class) {
  ruby_xml_sax_parser *rxsp;

  xmlSAXHandler emptySAXHandlerStruct = {
    NULL, /* internalSubset */
    NULL, /* isStandalone */
    NULL, /* hasInternalSubset */
    NULL, /* hasExternalSubset */
    NULL, /* resolveEntity */
    NULL, /* getEntity */
    NULL, /* entityDecl */
    NULL, /* notationDecl */
    NULL, /* attributeDecl */
    NULL, /* elementDecl */
    NULL, /* unparsedEntityDecl */
    NULL, /* setDocumentLocator */
    NULL, /* startDocument */
    NULL, /* endDocument */
    NULL, /* startElement */
    NULL, /* endElement */
    NULL, /* reference */
    NULL, /* characters */
    NULL, /* ignorableWhitespace */
    NULL, /* processingInstruction */
    NULL, /* comment */
    NULL, /* xmlParserWarning */
    NULL, /* xmlParserError */
    NULL, /* xmlParserError */
    NULL, /* getParameterEntity */
    NULL, /* cdataBlock; */
    NULL, /* externalSubset; */
    1
  };

  rxsp = ALLOC(ruby_xml_sax_parser);
  rxsp->xsh = &emptySAXHandlerStruct;

  rxsp->xpc = NULL;
  rxsp->filename = Qnil;
  rxsp->str = Qnil;

  return(Data_Wrap_Struct(class, 0, ruby_xml_sax_parser_free, rxsp));
}


VALUE
ruby_xml_sax_parser_filename_get(VALUE self) {
  ruby_xml_sax_parser *rxsp;
  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);
  return(rxsp->filename);
}


VALUE
ruby_xml_sax_parser_filename_set(VALUE self, VALUE filename) {
  ruby_xml_sax_parser *rxsp;
  Check_Type(filename, T_STRING);
  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);
  rxsp->filename = filename;
  return(rxsp->filename);
}


VALUE
ruby_xml_sax_parser_parse(VALUE self) {
  char *str;
  int status;
  ruby_xml_sax_parser *rxsp;
  VALUE docobj = Qnil;

  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);

  if (rxsp->filename != Qnil) {
    status = xmlSAXUserParseFile(rxsp->xsh, NULL, STR2CSTR(rxsp->filename));

    /* XXX This should return an exception for the various error codes
     * that can come back in status, but I'm too lazy to do that right
     * now. */
    if (status)
      docobj = Qfalse;
    else
      docobj = Qtrue;
  } else if (rxsp->str != Qnil) {
    str = STR2CSTR(rxsp->str);
    docobj = ruby_xml_document_new(cXMLDocument,
				   xmlSAXParseMemory(rxsp->xsh, str,
						     strlen(str), 0));
  }
  return(docobj);
}


VALUE
ruby_xml_sax_parser_str_get(VALUE self) {
  ruby_xml_sax_parser *rxsp;
  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);
  return(rxsp->str);
}


VALUE
ruby_xml_sax_parser_str_set(VALUE self, VALUE str) {
  ruby_xml_sax_parser *rxsp;
  Check_Type(str, T_STRING);
  Data_Get_Struct(self, ruby_xml_sax_parser, rxsp);
  rxsp->str = str;
  return(rxsp->str);
}


void
ruby_init_xml_sax_parser(void) {
  cXMLSaxParser = rb_define_class_under(mXML, "SaxParser", rb_cObject);

  rb_define_singleton_method(cXMLSaxParser, "new", ruby_xml_sax_parser_new, 0);

  rb_define_method(cXMLSaxParser, "filename",
		   ruby_xml_sax_parser_filename_get, 0);
  rb_define_method(cXMLSaxParser, "filename=",
		   ruby_xml_sax_parser_filename_set, 1);
  rb_define_method(cXMLSaxParser, "parse", ruby_xml_sax_parser_parse, 0);
  rb_define_method(cXMLSaxParser, "string", ruby_xml_sax_parser_str_get, 0);
  rb_define_method(cXMLSaxParser, "string=", ruby_xml_sax_parser_str_set, 1);
}
