# $Id: tc_xml_parser6.rb,v 1.1.1.1 2003/08/23 15:08:42 xpovolny Exp $

class TC_XML_Parser6 < Test::Unit::TestCase
  def setup()
    @xp = XML::Parser.new()
    assert_instance_of(XML::Parser, @xp)
    f = File.open('rubynet.xml')
    f1 = @xp.io = f
    assert_instance_of(File, f1)
    assert_instance_of(File, @xp.io)
  end

  def teardown()
    @xp = nil
  end

  def test_ruby_xml_parser_new()
    assert_instance_of(XML::Parser, @xp)
    assert_instance_of(File, @xp.io)
  end
end
