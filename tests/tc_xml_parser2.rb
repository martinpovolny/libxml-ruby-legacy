# $Id: tc_xml_parser2.rb,v 1.1.1.1 2003/08/23 15:08:42 xpovolny Exp $

class TC_XML_Parser2 < Test::Unit::TestCase
  def setup()
    @xp = XML::Parser.new()
  end

  def teardown()
    @xp = nil
  end

  def test_ruby_xml_parser_new()
    assert_instance_of(XML::Parser, @xp)
  end
end # TC_XML_Document
