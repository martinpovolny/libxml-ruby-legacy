require 'libxml'

xp = LibXML::Parser.new
xp.string = '<?xml version="1.0" encoding="utf-8"?>
<root><head a="ee" id="1">George Bush</head><descr>really big ass</descr></root>'

doc = xp.parse

dtd = LibXML::Dtd.new('
<!ELEMENT root (head, descr)>
<!ELEMENT head (#PCDATA)>
<!ATTLIST head
  id NMTOKEN #REQUIRED
  a CDATA #IMPLIED
>
<!ELEMENT descr (#PCDATA)>
')

if doc.validate(dtd) { |message, error| puts "#{error ? 'error' : 'warning'} : #{message}" } 
  puts "validation passed"
else
  puts "validation failed"
end

