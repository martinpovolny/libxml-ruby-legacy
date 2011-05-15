# $Id: tc_xml_xpath.rb,v 1.1.1.1 2003/08/23 15:08:42 xpovolny Exp $

class TC_XML_XPath < Test::Unit::TestCase
  def setup()
    xp = XML::Parser.new()
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, xp.string = str)
    doc = xp.parse
    assert_instance_of(XML::Document, doc)
    @xpt = doc.find('/ruby_array/fixnum').xpath
    assert_instance_of(XML::XPath, @xpt)
  end

  def teardown()
    @xpt = nil
  end

  def test_libxml_xpath_set()
    set = @xpt.set
    assert_instance_of(XML::Node::Set, set)    
  end
end # TC_XML_Document
