#!/usr/local/bin/ruby -w

require "./libxml"

classes = [ XML, XML::Attr, XML::Document, XML::Node, XML::Node::Set,
  XML::NS, XML::Parser, XML::Parser::Context, XML::SaxParser,
  XML::Tree, XML::XPath]

$class_constants  = 0
$class_methods    = 0
$instance_methods = 0



def print_class(c)
  puts c.inspect.to_s

  consts = c.constants
  for a in c.ancestors
    next if a == c
    consts -= a.constants
  end
  puts "\tClass Constants (#{consts.length}):\t\t" + consts.sort.inspect
  $class_constants += consts.length

  puts "\tPublic Class Methods (#{c.singleton_methods.length}):\t" + c.singleton_methods.sort.inspect
  $class_methods += c.singleton_methods.length

  puts "\tPublic Instance Methods (#{c.public_instance_methods.length}):\t" + c.public_instance_methods.sort.inspect
  $instance_methods += c.public_instance_methods.length
  puts ""
end

for c in classes
  print_class(c)
end

puts "Total Class Methods:\t#{$class_methods}"
puts "Total Class Constants:\t#{$class_constants}"
puts "Total Instance Methods:\t#{$instance_methods}"
