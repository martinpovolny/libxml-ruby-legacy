# $Id: tc_xml_node_set.rb,v 1.1.1.1 2003/08/23 15:08:42 xpovolny Exp $

class TC_XML_Node_Set < Test::Unit::TestCase
  def setup()
    xp = XML::Parser.new()
    str = '<ruby_array uga="booga" foo="bar"><fixnum>one</fixnum><fixnum>two</fixnum></ruby_array>'
    assert_equal(str, xp.string = str)
    doc = xp.parse
    assert_instance_of(XML::Document, doc)
    @set = doc.find('/ruby_array/fixnum')
    assert_instance_of(XML::Node::Set, @set)    
  end

  def teardown()
    @set = nil
  end

  def test_libxml_nodeset_each()
    @set.each do |n|
      assert_instance_of(XML::Node, n)
    end
  end
end # TC_XML_Document
