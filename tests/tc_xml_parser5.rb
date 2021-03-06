# $Id: tc_xml_parser5.rb,v 1.1.1.1 2003/08/23 15:08:42 xpovolny Exp $

class TC_XML_Parser5 < Test::Unit::TestCase
  def setup()
    @xp = XML::Parser.new()
    assert_instance_of(XML::Parser, @xp)
    s = @xp.string = '<u>a</u>'
    assert_instance_of(String, s)
    assert_instance_of(String, @xp.string)
    assert_equal(s, @xp.string)
    @doc = @xp.parse
    assert_instance_of(XML::Document, @doc)
  end

  def teardown()
    @xp = nil
    @doc = nil
  end

  def test_ruby_xml_parser_new()
    assert_instance_of(XML::Parser, @xp)
    assert_instance_of(String, @xp.string)
    assert_instance_of(XML::Document, @doc)
  end
end
