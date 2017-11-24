/*
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/
#include "StdAfx.h"
#include "MyFile_XML.h"
using namespace tinyxml2; 


#include <new>		// yes, this one new style header, is in the Android SDK.
#   ifdef ANDROID_NDK
#   include <stddef.h>
#else
#   include <cstddef>
#endif

static const char LINE_FEED				= (char)0x0a;			// all line endings are normalized to LF
static const char LF = LINE_FEED;
static const char CARRIAGE_RETURN		= (char)0x0d;			// CR gets filtered out
static const char CR = CARRIAGE_RETURN;
static const char SINGLE_QUOTE			= '\'';
static const char DOUBLE_QUOTE			= '\"';

// Bunch of unicode info at:
//		http://www.unicode.org/faq/utf_bom.html
//	ef bb bf (Microsoft "lead bytes") - designates UTF-8

static const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
static const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
static const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;


#define DELETE_NODE( node )	{			\
        if ( node ) {						\
            MemPool* pool = node->_memPool;	\
            node->~XMLNode();				\
            pool->Free( node );				\
        }									\
    }
#define DELETE_ATTRIBUTE( attrib ) {		\
        if ( attrib ) {							\
            MemPool* pool = attrib->_memPool;	\
            attrib->~XMLAttribute();			\
            pool->Free( attrib );				\
        }										\
    }

namespace tinyxml2
{

struct Entity {
    const char* pattern;
    int length;
    char value;
};

static const int NUM_ENTITIES = 5;
static const Entity entities[NUM_ENTITIES] = {
    { "quot", 4,	DOUBLE_QUOTE },
    { "amp", 3,		'&'  },
    { "apos", 4,	SINGLE_QUOTE },
    { "lt",	2, 		'<'	 },
    { "gt",	2,		'>'	 }
};


StrPair::~StrPair()
{
    Reset();
}


void StrPair::Reset()
{
    if ( _flags & NEEDS_DELETE ) {
        delete [] _start;
    }
    _flags = 0;
    _start = 0;
    _end = 0;
}


void StrPair::SetStr( const char* str, int flags )
{
    Reset();
    size_t len = strlen( str );
    _start = new char[ len+1 ];
    memcpy( _start, str, len+1 );
    _end = _start + len;
    _flags = flags | NEEDS_DELETE;
}


char* StrPair::ParseText( char* p, const char* endTag, int strFlags )
{
    TIXMLASSERT( endTag && *endTag );

    char* start = p;	// fixme: hides a member
    char  endChar = *endTag;
    size_t length = strlen( endTag );

    // Inner loop of text parsing.
    while ( *p ) {
        if ( *p == endChar && strncmp( p, endTag, length ) == 0 ) {
            Set( start, p, strFlags );
            return p + length;
        }
        ++p;
    }
    return 0;
}


char* StrPair::ParseName( char* p )
{
    char* start = p;

    if ( !start || !(*start) ) {
        return 0;
    }

    while( *p && ( p == start ? XMLUtil::IsNameStartChar( *p ) : XMLUtil::IsNameChar( *p ) )) {
        ++p;
    }

    if ( p > start ) {
        Set( start, p, 0 );
        return p;
    }
    return 0;
}


void StrPair::CollapseWhitespace()
{
    // Trim leading space.
    _start = XMLUtil::SkipWhiteSpace( _start );

    if ( _start && *_start ) {
        char* p = _start;	// the read pointer
        char* q = _start;	// the write pointer

        while( *p ) {
            if ( XMLUtil::IsWhiteSpace( *p )) {
                p = XMLUtil::SkipWhiteSpace( p );
                if ( *p == 0 ) {
                    break;    // don't write to q; this trims the trailing space.
                }
                *q = ' ';
                ++q;
            }
            *q = *p;
            ++q;
            ++p;
        }
        *q = 0;
    }
}


const char* StrPair::GetStr()
{
    if ( _flags & NEEDS_FLUSH ) {
        *_end = 0;
        _flags ^= NEEDS_FLUSH;

        if ( _flags ) {
            char* p = _start;	// the read pointer
            char* q = _start;	// the write pointer

            while( p < _end ) {
                if ( (_flags & NEEDS_NEWLINE_NORMALIZATION) && *p == CR ) {
                    // CR-LF pair becomes LF
                    // CR alone becomes LF
                    // LF-CR becomes LF
                    if ( *(p+1) == LF ) {
                        p += 2;
                    }
                    else {
                        ++p;
                    }
                    *q++ = LF;
                }
                else if ( (_flags & NEEDS_NEWLINE_NORMALIZATION) && *p == LF ) {
                    if ( *(p+1) == CR ) {
                        p += 2;
                    }
                    else {
                        ++p;
                    }
                    *q++ = LF;
                }
                else if ( (_flags & NEEDS_ENTITY_PROCESSING) && *p == '&' ) {
                    // Entities handled by tinyXML2:
                    // - special entities in the entity table [in/out]
                    // - numeric character reference [in]
                    //   &#20013; or &#x4e2d;

                    if ( *(p+1) == '#' ) {
                        char buf[10] = { 0 };
                        int len;
                        p = const_cast<char*>( XMLUtil::GetCharacterRef( p, buf, &len ) );
                        for( int i=0; i<len; ++i ) {
                            *q++ = buf[i];
                        }
                        TIXMLASSERT( q <= p );
                    }
                    else {
                        int i=0;
                        for(; i<NUM_ENTITIES; ++i ) {
                            if (    strncmp( p+1, entities[i].pattern, entities[i].length ) == 0
                                    && *(p+entities[i].length+1) == ';' ) {
                                // Found an entity convert;
                                *q = entities[i].value;
                                ++q;
                                p += entities[i].length + 2;
                                break;
                            }
                        }
                        if ( i == NUM_ENTITIES ) {
                            // fixme: treat as error?
                            ++p;
                            ++q;
                        }
                    }
                }
                else {
                    *q = *p;
                    ++p;
                    ++q;
                }
            }
            *q = 0;
        }
        // The loop below has plenty going on, and this
        // is a less useful mode. Break it out.
        if ( _flags & COLLAPSE_WHITESPACE ) {
            CollapseWhitespace();
        }
        _flags = (_flags & NEEDS_DELETE);
    }
    return _start;
}




// --------- XMLUtil ----------- //

const char* XMLUtil::ReadBOM( const char* p, bool* bom )
{
    *bom = false;
    const unsigned char* pu = reinterpret_cast<const unsigned char*>(p);
    // Check for BOM:
    if (    *(pu+0) == TIXML_UTF_LEAD_0
            && *(pu+1) == TIXML_UTF_LEAD_1
            && *(pu+2) == TIXML_UTF_LEAD_2 ) {
        *bom = true;
        p += 3;
    }
    return p;
}


void XMLUtil::ConvertUTF32ToUTF8( long input, char* output, int* length )
{
    const unsigned long BYTE_MASK = 0xBF;
    const unsigned long BYTE_MARK = 0x80;
    const unsigned long FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

    if (input < 0x80) {
        *length = 1;
    }
    else if ( input < 0x800 ) {
        *length = 2;
    }
    else if ( input < 0x10000 ) {
        *length = 3;
    }
    else if ( input < 0x200000 ) {
        *length = 4;
    }
    else {
        *length = 0;    // This code won't covert this correctly anyway.
        return;
    }

    output += *length;

    // Scary scary fall throughs.
    switch (*length) {
        case 4:
            --output;
            *output = (char)((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
        case 3:
            --output;
            *output = (char)((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
        case 2:
            --output;
            *output = (char)((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
        case 1:
            --output;
            *output = (char)(input | FIRST_BYTE_MARK[*length]);
        default:
            break;
    }
}


const char* XMLUtil::GetCharacterRef( const char* p, char* value, int* length )
{
    // Presume an entity, and pull it out.
    *length = 0;

    if ( *(p+1) == '#' && *(p+2) ) {
        unsigned long ucs = 0;
        ptrdiff_t delta = 0;
        unsigned mult = 1;

        if ( *(p+2) == 'x' ) {
            // Hexadecimal.
            if ( !*(p+3) ) {
                return 0;
            }

            const char* q = p+3;
            q = strchr( q, ';' );

            if ( !q || !*q ) {
                return 0;
            }

            delta = q-p;
            --q;

            while ( *q != 'x' ) {
                if ( *q >= '0' && *q <= '9' ) {
                    ucs += mult * (*q - '0');
                }
                else if ( *q >= 'a' && *q <= 'f' ) {
                    ucs += mult * (*q - 'a' + 10);
                }
                else if ( *q >= 'A' && *q <= 'F' ) {
                    ucs += mult * (*q - 'A' + 10 );
                }
                else {
                    return 0;
                }
                mult *= 16;
                --q;
            }
        }
        else {
            // Decimal.
            if ( !*(p+2) ) {
                return 0;
            }

            const char* q = p+2;
            q = strchr( q, ';' );

            if ( !q || !*q ) {
                return 0;
            }

            delta = q-p;
            --q;

            while ( *q != '#' ) {
                if ( *q >= '0' && *q <= '9' ) {
                    ucs += mult * (*q - '0');
                }
                else {
                    return 0;
                }
                mult *= 10;
                --q;
            }
        }
        // convert the UCS to UTF-8
        ConvertUTF32ToUTF8( ucs, value, length );
        return p + delta + 1;
    }
    return p+1;
}


void XMLUtil::ToStr( int v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%d", v );
}


void XMLUtil::ToStr( unsigned v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%u", v );
}


void XMLUtil::ToStr( bool v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%d", v ? 1 : 0 );
}


void XMLUtil::ToStr( float v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%f", v );
}


void XMLUtil::ToStr( double v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%f", v );
}


bool XMLUtil::ToInt( const char* str, int* value )
{
    if ( TIXML_SSCANF( str, "%d", value ) == 1 ) {
        return true;
    }
    return false;
}

bool XMLUtil::ToUnsigned( const char* str, unsigned *value )
{
    if ( TIXML_SSCANF( str, "%u", value ) == 1 ) {
        return true;
    }
    return false;
}

bool XMLUtil::ToBool( const char* str, bool* value )
{
    int ival = 0;
    if ( ToInt( str, &ival )) {
        *value = (ival==0) ? false : true;
        return true;
    }
    if ( StringEqual( str, "true" ) ) {
        *value = true;
        return true;
    }
    else if ( StringEqual( str, "false" ) ) {
        *value = false;
        return true;
    }
    return false;
}


bool XMLUtil::ToFloat( const char* str, float* value )
{
    if ( TIXML_SSCANF( str, "%f", value ) == 1 ) {
        return true;
    }
    return false;
}

bool XMLUtil::ToDouble( const char* str, double* value )
{
    if ( TIXML_SSCANF( str, "%lf", value ) == 1 ) {
        return true;
    }
    return false;
}


char* MyXMLDocument::Identify( char* p, XMLNode** node )
{
    XMLNode* returnNode = 0;
    char* start = p;
    p = XMLUtil::SkipWhiteSpace( p );
    if( !p || !*p ) {
        return p;
    }

    // What is this thing?
    // - Elements start with a letter or underscore, but xml is reserved.
    // - Comments: <!--
    // - Declaration: <?
    // - Everything else is unknown to tinyxml.
    //

    static const char* xmlHeader		= { "<?" };
    static const char* commentHeader	= { "<!--" };
    static const char* dtdHeader		= { "<!" };
    static const char* cdataHeader		= { "<![CDATA[" };
    static const char* elementHeader	= { "<" };	// and a header for everything else; check last.

    static const int xmlHeaderLen		= 2;
    static const int commentHeaderLen	= 4;
    static const int dtdHeaderLen		= 2;
    static const int cdataHeaderLen		= 9;
    static const int elementHeaderLen	= 1;

#if defined(_MSC_VER)
#pragma warning ( push )
#pragma warning ( disable : 4127 )
#endif
    TIXMLASSERT( sizeof( XMLComment ) == sizeof( XMLUnknown ) );		// use same memory pool
    TIXMLASSERT( sizeof( XMLComment ) == sizeof( XMLDeclaration ) );	// use same memory pool
#if defined(_MSC_VER)
#pragma warning (pop)
#endif
    if ( XMLUtil::StringEqual( p, xmlHeader, xmlHeaderLen ) ) {
        returnNode = new (_commentPool.Alloc()) XMLDeclaration( this );
        returnNode->_memPool = &_commentPool;
        p += xmlHeaderLen;
    }
    else if ( XMLUtil::StringEqual( p, commentHeader, commentHeaderLen ) ) {
        returnNode = new (_commentPool.Alloc()) XMLComment( this );
        returnNode->_memPool = &_commentPool;
        p += commentHeaderLen;
    }
    else if ( XMLUtil::StringEqual( p, cdataHeader, cdataHeaderLen ) ) {
        XMLText* text = new (_textPool.Alloc()) XMLText( this );
        returnNode = text;
        returnNode->_memPool = &_textPool;
        p += cdataHeaderLen;
        text->SetCData( true );
    }
    else if ( XMLUtil::StringEqual( p, dtdHeader, dtdHeaderLen ) ) {
        returnNode = new (_commentPool.Alloc()) XMLUnknown( this );
        returnNode->_memPool = &_commentPool;
        p += dtdHeaderLen;
    }
    else if ( XMLUtil::StringEqual( p, elementHeader, elementHeaderLen ) ) {
        returnNode = new (_elementPool.Alloc()) XMLElement( this );
        returnNode->_memPool = &_elementPool;
        p += elementHeaderLen;
    }
    else {
        returnNode = new (_textPool.Alloc()) XMLText( this );
        returnNode->_memPool = &_textPool;
        p = start;	// Back it up, all the text counts.
    }

    *node = returnNode;
    return p;
}


bool MyXMLDocument::Accept( XMLVisitor* visitor ) const
{
    if ( visitor->VisitEnter( *this ) ) {
        for ( const XMLNode* node=FirstChild(); node; node=node->NextSibling() ) {
            if ( !node->Accept( visitor ) ) {
                break;
            }
        }
    }
    return visitor->VisitExit( *this );
}


// --------- XMLNode ----------- //

XMLNode::XMLNode( MyXMLDocument* doc ) :
    _document( doc ),
    _parent( 0 ),
    _firstChild( 0 ), _lastChild( 0 ),
    _prev( 0 ), _next( 0 ),
    _memPool( 0 )
{
}


XMLNode::~XMLNode()
{
    DeleteChildren();
    if ( _parent ) {
        _parent->Unlink( this );
    }
}


void XMLNode::SetValue( const char* str, bool staticMem )
{
    if ( staticMem ) {
        _value.SetInternedStr( str );
    }
    else {
        _value.SetStr( str );
    }
}


void XMLNode::DeleteChildren()
{
    while( _firstChild ) {
        XMLNode* node = _firstChild;
        Unlink( node );

        DELETE_NODE( node );
    }
    _firstChild = _lastChild = 0;
}


void XMLNode::Unlink( XMLNode* child )
{
    TIXMLASSERT( child->_parent == this );
    if ( child == _firstChild ) {
        _firstChild = _firstChild->_next;
    }
    if ( child == _lastChild ) {
        _lastChild = _lastChild->_prev;
    }

    if ( child->_prev ) {
        child->_prev->_next = child->_next;
    }
    if ( child->_next ) {
        child->_next->_prev = child->_prev;
    }
    child->_parent = 0;
}


void XMLNode::DeleteChild( XMLNode* node )
{
    TIXMLASSERT( node->_parent == this );
    DELETE_NODE( node );
}


XMLNode* XMLNode::InsertEndChild( XMLNode* addThis )
{
    if ( _lastChild ) {
        TIXMLASSERT( _firstChild );
        TIXMLASSERT( _lastChild->_next == 0 );
        _lastChild->_next = addThis;
        addThis->_prev = _lastChild;
        _lastChild = addThis;

        addThis->_next = 0;
    }
    else {
        TIXMLASSERT( _firstChild == 0 );
        _firstChild = _lastChild = addThis;

        addThis->_prev = 0;
        addThis->_next = 0;
    }
    addThis->_parent = this;
    addThis->_memPool->SetTracked();
    return addThis;
}


XMLNode* XMLNode::InsertFirstChild( XMLNode* addThis )
{
    if ( _firstChild ) {
        TIXMLASSERT( _lastChild );
        TIXMLASSERT( _firstChild->_prev == 0 );

        _firstChild->_prev = addThis;
        addThis->_next = _firstChild;
        _firstChild = addThis;

        addThis->_prev = 0;
    }
    else {
        TIXMLASSERT( _lastChild == 0 );
        _firstChild = _lastChild = addThis;

        addThis->_prev = 0;
        addThis->_next = 0;
    }
    addThis->_parent = this;
    addThis->_memPool->SetTracked();
    return addThis;
}


XMLNode* XMLNode::InsertAfterChild( XMLNode* afterThis, XMLNode* addThis )
{
    TIXMLASSERT( afterThis->_parent == this );
    if ( afterThis->_parent != this ) {
        return 0;
    }

    if ( afterThis->_next == 0 ) {
        // The last node or the only node.
        return InsertEndChild( addThis );
    }
    addThis->_prev = afterThis;
    addThis->_next = afterThis->_next;
    afterThis->_next->_prev = addThis;
    afterThis->_next = addThis;
    addThis->_parent = this;
    addThis->_memPool->SetTracked();
    return addThis;
}


const XMLElement* XMLNode::FirstChildElement( const char* value ) const
{
    for( XMLNode* node=_firstChild; node; node=node->_next ) {
        XMLElement* element = node->ToElement();
        if ( element ) {
            if ( !value || XMLUtil::StringEqual( element->Name(), value ) ) {
                return element;
            }
        }
    }
    return 0;
}


const XMLElement* XMLNode::LastChildElement( const char* value ) const
{
    for( XMLNode* node=_lastChild; node; node=node->_prev ) {
        XMLElement* element = node->ToElement();
        if ( element ) {
            if ( !value || XMLUtil::StringEqual( element->Name(), value ) ) {
                return element;
            }
        }
    }
    return 0;
}


const XMLElement* XMLNode::NextSiblingElement( const char* value ) const
{
    for( XMLNode* element=this->_next; element; element = element->_next ) {
        if (    element->ToElement()
                && (!value || XMLUtil::StringEqual( value, element->Value() ))) {
            return element->ToElement();
        }
    }
    return 0;
}


const XMLElement* XMLNode::PreviousSiblingElement( const char* value ) const
{
    for( XMLNode* element=_prev; element; element = element->_prev ) {
        if (    element->ToElement()
                && (!value || XMLUtil::StringEqual( value, element->Value() ))) {
            return element->ToElement();
        }
    }
    return 0;
}


char* XMLNode::ParseDeep( char* p, StrPair* parentEnd )
{
    // This is a recursive method, but thinking about it "at the current level"
    // it is a pretty simple flat list:
    //		<foo/>
    //		<!-- comment -->
    //
    // With a special case:
    //		<foo>
    //		</foo>
    //		<!-- comment -->
    //
    // Where the closing element (/foo) *must* be the next thing after the opening
    // element, and the names must match. BUT the tricky bit is that the closing
    // element will be read by the child.
    //
    // 'endTag' is the end tag for this node, it is returned by a call to a child.
    // 'parentEnd' is the end tag for the parent, which is filled in and returned.

    while( p && *p ) {
        XMLNode* node = 0;

        p = _document->Identify( p, &node );
        if ( p == 0 || node == 0 ) {
            break;
        }

        StrPair endTag;
        p = node->ParseDeep( p, &endTag );
        if ( !p ) {
            DELETE_NODE( node );
            node = 0;
            if ( !_document->Error() ) {
                _document->SetError( XML_ERROR_PARSING, 0, 0 );
            }
            break;
        }

        // We read the end tag. Return it to the parent.
        if ( node->ToElement() && node->ToElement()->ClosingType() == XMLElement::CLOSING ) {
            if ( parentEnd ) {
                *parentEnd = static_cast<XMLElement*>(node)->_value;
            }
			node->_memPool->SetTracked();	// created and then immediately deleted.
            DELETE_NODE( node );
            return p;
        }

        // Handle an end tag returned to this level.
        // And handle a bunch of annoying errors.
        XMLElement* ele = node->ToElement();
        if ( ele ) {
            if ( endTag.Empty() && ele->ClosingType() == XMLElement::OPEN ) {
                _document->SetError( XML_ERROR_MISMATCHED_ELEMENT, node->Value(), 0 );
                p = 0;
            }
            else if ( !endTag.Empty() && ele->ClosingType() != XMLElement::OPEN ) {
                _document->SetError( XML_ERROR_MISMATCHED_ELEMENT, node->Value(), 0 );
                p = 0;
            }
            else if ( !endTag.Empty() ) {
                if ( !XMLUtil::StringEqual( endTag.GetStr(), node->Value() )) {
                    _document->SetError( XML_ERROR_MISMATCHED_ELEMENT, node->Value(), 0 );
                    p = 0;
                }
            }
        }
        if ( p == 0 ) {
            DELETE_NODE( node );
            node = 0;
        }
        if ( node ) {
            this->InsertEndChild( node );
        }
    }
    return 0;
}

// --------- XMLText ---------- //
char* XMLText::ParseDeep( char* p, StrPair* )
{
    const char* start = p;
    if ( this->CData() ) {
        p = _value.ParseText( p, "]]>", StrPair::NEEDS_NEWLINE_NORMALIZATION );
        if ( !p ) {
            _document->SetError( XML_ERROR_PARSING_CDATA, start, 0 );
        }
        return p;
    }
    else {
        int flags = _document->ProcessEntities() ? StrPair::TEXT_ELEMENT : StrPair::TEXT_ELEMENT_LEAVE_ENTITIES;
        if ( _document->WhitespaceMode() == COLLAPSE_WHITESPACE ) {
            flags |= StrPair::COLLAPSE_WHITESPACE;
        }

        p = _value.ParseText( p, "<", flags );
        if ( !p ) {
            _document->SetError( XML_ERROR_PARSING_TEXT, start, 0 );
        }
        if ( p && *p ) {
            return p-1;
        }
    }
    return 0;
}


XMLNode* XMLText::ShallowClone( MyXMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLText* text = doc->NewText( Value() );	// fixme: this will always allocate memory. Intern?
    text->SetCData( this->CData() );
    return text;
}


bool XMLText::ShallowEqual( const XMLNode* compare ) const
{
    return ( compare->ToText() && XMLUtil::StringEqual( compare->ToText()->Value(), Value() ));
}


bool XMLText::Accept( XMLVisitor* visitor ) const
{
    return visitor->Visit( *this );
}


// --------- XMLComment ---------- //

XMLComment::XMLComment( MyXMLDocument* doc ) : XMLNode( doc )
{
}


XMLComment::~XMLComment()
{
}


char* XMLComment::ParseDeep( char* p, StrPair* )
{
    // Comment parses as text.
    const char* start = p;
    p = _value.ParseText( p, "-->", StrPair::COMMENT );
    if ( p == 0 ) {
        _document->SetError( XML_ERROR_PARSING_COMMENT, start, 0 );
    }
    return p;
}


XMLNode* XMLComment::ShallowClone( MyXMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLComment* comment = doc->NewComment( Value() );	// fixme: this will always allocate memory. Intern?
    return comment;
}


bool XMLComment::ShallowEqual( const XMLNode* compare ) const
{
    return ( compare->ToComment() && XMLUtil::StringEqual( compare->ToComment()->Value(), Value() ));
}


bool XMLComment::Accept( XMLVisitor* visitor ) const
{
    return visitor->Visit( *this );
}


// --------- XMLDeclaration ---------- //

XMLDeclaration::XMLDeclaration( MyXMLDocument* doc ) : XMLNode( doc )
{
}


XMLDeclaration::~XMLDeclaration()
{
    //printf( "~XMLDeclaration\n" );
}


char* XMLDeclaration::ParseDeep( char* p, StrPair* )
{
    // Declaration parses as text.
    const char* start = p;
    p = _value.ParseText( p, "?>", StrPair::NEEDS_NEWLINE_NORMALIZATION );
    if ( p == 0 ) {
        _document->SetError( XML_ERROR_PARSING_DECLARATION, start, 0 );
    }
    return p;
}


XMLNode* XMLDeclaration::ShallowClone( MyXMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLDeclaration* dec = doc->NewDeclaration( Value() );	// fixme: this will always allocate memory. Intern?
    return dec;
}


bool XMLDeclaration::ShallowEqual( const XMLNode* compare ) const
{
    return ( compare->ToDeclaration() && XMLUtil::StringEqual( compare->ToDeclaration()->Value(), Value() ));
}



bool XMLDeclaration::Accept( XMLVisitor* visitor ) const
{
    return visitor->Visit( *this );
}

// --------- XMLUnknown ---------- //

XMLUnknown::XMLUnknown( MyXMLDocument* doc ) : XMLNode( doc )
{
}


XMLUnknown::~XMLUnknown()
{
}


char* XMLUnknown::ParseDeep( char* p, StrPair* )
{
    // Unknown parses as text.
    const char* start = p;

    p = _value.ParseText( p, ">", StrPair::NEEDS_NEWLINE_NORMALIZATION );
    if ( !p ) {
        _document->SetError( XML_ERROR_PARSING_UNKNOWN, start, 0 );
    }
    return p;
}


XMLNode* XMLUnknown::ShallowClone( MyXMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLUnknown* text = doc->NewUnknown( Value() );	// fixme: this will always allocate memory. Intern?
    return text;
}


bool XMLUnknown::ShallowEqual( const XMLNode* compare ) const
{
    return ( compare->ToUnknown() && XMLUtil::StringEqual( compare->ToUnknown()->Value(), Value() ));
}


bool XMLUnknown::Accept( XMLVisitor* visitor ) const
{
    return visitor->Visit( *this );
}

// --------- XMLAttribute ---------- //
char* XMLAttribute::ParseDeep( char* p, bool processEntities )
{
    // Parse using the name rules: bug fix, was using ParseText before
    p = _name.ParseName( p );
    if ( !p || !*p ) {
        return 0;
    }

    // Skip white space before =
    p = XMLUtil::SkipWhiteSpace( p );
    if ( !p || *p != '=' ) {
        return 0;
    }

    ++p;	// move up to opening quote
    p = XMLUtil::SkipWhiteSpace( p );
    if ( *p != '\"' && *p != '\'' ) {
        return 0;
    }

    char endTag[2] = { *p, 0 };
    ++p;	// move past opening quote

    p = _value.ParseText( p, endTag, processEntities ? StrPair::ATTRIBUTE_VALUE : StrPair::ATTRIBUTE_VALUE_LEAVE_ENTITIES );
    return p;
}


void XMLAttribute::SetName( const char* n )
{
    _name.SetStr( n );
}


XMLError XMLAttribute::QueryIntValue( int* value ) const
{
    if ( XMLUtil::ToInt( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryUnsignedValue( unsigned int* value ) const
{
    if ( XMLUtil::ToUnsigned( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryBoolValue( bool* value ) const
{
    if ( XMLUtil::ToBool( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryFloatValue( float* value ) const
{
    if ( XMLUtil::ToFloat( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryDoubleValue( double* value ) const
{
    if ( XMLUtil::ToDouble( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


void XMLAttribute::SetAttribute( const char* v )
{
    _value.SetStr( v );
}


void XMLAttribute::SetAttribute( int v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


void XMLAttribute::SetAttribute( unsigned v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


void XMLAttribute::SetAttribute( bool v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}

void XMLAttribute::SetAttribute( double v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}

void XMLAttribute::SetAttribute( float v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


// --------- XMLElement ---------- //
XMLElement::XMLElement( MyXMLDocument* doc ) : XMLNode( doc ),
    _closingType( 0 ),
    _rootAttribute( 0 )
{
}


XMLElement::~XMLElement()
{
    while( _rootAttribute ) {
        XMLAttribute* next = _rootAttribute->_next;
        DELETE_ATTRIBUTE( _rootAttribute );
        _rootAttribute = next;
    }
}


XMLAttribute* XMLElement::FindAttribute( const char* name )
{
    XMLAttribute* a = 0;
    for( a=_rootAttribute; a; a = a->_next ) {
        if ( XMLUtil::StringEqual( a->Name(), name ) ) {
            return a;
        }
    }
    return 0;
}


const XMLAttribute* XMLElement::FindAttribute( const char* name ) const
{
    XMLAttribute* a = 0;
    for( a=_rootAttribute; a; a = a->_next ) {
        if ( XMLUtil::StringEqual( a->Name(), name ) ) {
            return a;
        }
    }
    return 0;
}


const char* XMLElement::Attribute( const char* name, const char* value ) const
{
    const XMLAttribute* a = FindAttribute( name );
    if ( !a ) {
        return 0;
    }
    if ( !value || XMLUtil::StringEqual( a->Value(), value )) {
        return a->Value();
    }
    return 0;
}


const char* XMLElement::GetText() const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        return FirstChild()->ToText()->Value();
    }
    return "";
}


XMLError XMLElement::QueryIntText( int* ival ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->ToText()->Value();
        if ( XMLUtil::ToInt( t, ival ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryUnsignedText( unsigned* uval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->ToText()->Value();
        if ( XMLUtil::ToUnsigned( t, uval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryBoolText( bool* bval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->ToText()->Value();
        if ( XMLUtil::ToBool( t, bval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryDoubleText( double* dval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->ToText()->Value();
        if ( XMLUtil::ToDouble( t, dval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryFloatText( float* fval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->ToText()->Value();
        if ( XMLUtil::ToFloat( t, fval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}



XMLAttribute* XMLElement::FindOrCreateAttribute( const char* name )
{
    XMLAttribute* last = 0;
    XMLAttribute* attrib = 0;
    for( attrib = _rootAttribute;
            attrib;
            last = attrib, attrib = attrib->_next ) {
        if ( XMLUtil::StringEqual( attrib->Name(), name ) ) {
            break;
        }
    }
    if ( !attrib ) {
        attrib = new (_document->_attributePool.Alloc() ) XMLAttribute();
        attrib->_memPool = &_document->_attributePool;
        if ( last ) {
            last->_next = attrib;
        }
        else {
            _rootAttribute = attrib;
        }
        attrib->SetName( name );
        attrib->_memPool->SetTracked(); // always created and linked.
    }
    return attrib;
}


void XMLElement::DeleteAttribute( const char* name )
{
    XMLAttribute* prev = 0;
    for( XMLAttribute* a=_rootAttribute; a; a=a->_next ) {
        if ( XMLUtil::StringEqual( name, a->Name() ) ) {
            if ( prev ) {
                prev->_next = a->_next;
            }
            else {
                _rootAttribute = a->_next;
            }
            DELETE_ATTRIBUTE( a );
            break;
        }
        prev = a;
    }
}


char* XMLElement::ParseAttributes( char* p )
{
    const char* start = p;
    XMLAttribute* prevAttribute = 0;

    // Read the attributes.
    while( p ) {
        p = XMLUtil::SkipWhiteSpace( p );
        if ( !p || !(*p) ) {
            _document->SetError( XML_ERROR_PARSING_ELEMENT, start, Name() );
            return 0;
        }

        // attribute.
        if (XMLUtil::IsNameStartChar( *p ) ) {
            XMLAttribute* attrib = new (_document->_attributePool.Alloc() ) XMLAttribute();
            attrib->_memPool = &_document->_attributePool;
			attrib->_memPool->SetTracked();

            p = attrib->ParseDeep( p, _document->ProcessEntities() );
            if ( !p || Attribute( attrib->Name() ) ) {
                DELETE_ATTRIBUTE( attrib );
                _document->SetError( XML_ERROR_PARSING_ATTRIBUTE, start, p );
                return 0;
            }
            // There is a minor bug here: if the attribute in the source xml
            // document is duplicated, it will not be detected and the
            // attribute will be doubly added. However, tracking the 'prevAttribute'
            // avoids re-scanning the attribute list. Preferring performance for
            // now, may reconsider in the future.
            if ( prevAttribute ) {
                prevAttribute->_next = attrib;
            }
            else {
                _rootAttribute = attrib;
            }
            prevAttribute = attrib;
        }
        // end of the tag
        else if ( *p == '/' && *(p+1) == '>' ) {
            _closingType = CLOSED;
            return p+2;	// done; sealed element.
        }
        // end of the tag
        else if ( *p == '>' ) {
            ++p;
            break;
        }
        else {
            _document->SetError( XML_ERROR_PARSING_ELEMENT, start, p );
            return 0;
        }
    }
    return p;
}


//
//	<ele></ele>
//	<ele>foo<b>bar</b></ele>
//
char* XMLElement::ParseDeep( char* p, StrPair* strPair )
{
    // Read the element name.
    p = XMLUtil::SkipWhiteSpace( p );
    if ( !p ) {
        return 0;
    }

    // The closing element is the </element> form. It is
    // parsed just like a regular element then deleted from
    // the DOM.
    if ( *p == '/' ) {
        _closingType = CLOSING;
        ++p;
    }

    p = _value.ParseName( p );
    if ( _value.Empty() ) {
        return 0;
    }

    p = ParseAttributes( p );
    if ( !p || !*p || _closingType ) {
        return p;
    }

    p = XMLNode::ParseDeep( p, strPair );
    return p;
}



XMLNode* XMLElement::ShallowClone( MyXMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLElement* element = doc->NewElement( Value() );					// fixme: this will always allocate memory. Intern?
    for( const XMLAttribute* a=FirstAttribute(); a; a=a->Next() ) {
        element->SetAttribute( a->Name(), a->Value() );					// fixme: this will always allocate memory. Intern?
    }
    return element;
}


bool XMLElement::ShallowEqual( const XMLNode* compare ) const
{
    const XMLElement* other = compare->ToElement();
    if ( other && XMLUtil::StringEqual( other->Value(), Value() )) {

        const XMLAttribute* a=FirstAttribute();
        const XMLAttribute* b=other->FirstAttribute();

        while ( a && b ) {
            if ( !XMLUtil::StringEqual( a->Value(), b->Value() ) ) {
                return false;
            }
            a = a->Next();
            b = b->Next();
        }
        if ( a || b ) {
            // different count
            return false;
        }
        return true;
    }
    return false;
}


bool XMLElement::Accept( XMLVisitor* visitor ) const
{
    if ( visitor->VisitEnter( *this, _rootAttribute ) ) {
        for ( const XMLNode* node=FirstChild(); node; node=node->NextSibling() ) {
            if ( !node->Accept( visitor ) ) {
                break;
            }
        }
    }
    return visitor->VisitExit( *this );
}


// --------- MyXMLDocument ----------- //
MyXMLDocument::MyXMLDocument( bool processEntities, Whitespace whitespace ) :
    XMLNode( 0 ),
    _writeBOM( false ),
    _processEntities( processEntities ),
    _errorID( XML_NO_ERROR ),
    _whitespace( whitespace ),
    _errorStr1( 0 ),
    _errorStr2( 0 ),
    _charBuffer( 0 )
{
    _document = this;	// avoid warning about 'this' in initializer list
}


MyXMLDocument::~MyXMLDocument()
{
    DeleteChildren();
    delete [] _charBuffer;

#if 0
    _textPool.Trace( "text" );
    _elementPool.Trace( "element" );
    _commentPool.Trace( "comment" );
    _attributePool.Trace( "attribute" );
#endif

#ifdef DEBUG
	if ( Error() == false ) {
		TIXMLASSERT( _elementPool.CurrentAllocs()   == _elementPool.Untracked() );
		TIXMLASSERT( _attributePool.CurrentAllocs() == _attributePool.Untracked() );
		TIXMLASSERT( _textPool.CurrentAllocs()      == _textPool.Untracked() );
		TIXMLASSERT( _commentPool.CurrentAllocs()   == _commentPool.Untracked() );
	}
#endif
}


void MyXMLDocument::Clear()
{
    DeleteChildren();

    _errorID = XML_NO_ERROR;
    _errorStr1 = 0;
    _errorStr2 = 0;

    delete [] _charBuffer;
    _charBuffer = 0;
}


XMLElement* MyXMLDocument::NewElement( const char* name )
{
    XMLElement* ele = new (_elementPool.Alloc()) XMLElement( this );
    ele->_memPool = &_elementPool;
    ele->SetName( name );
    return ele;
}


XMLComment* MyXMLDocument::NewComment( const char* str )
{
    XMLComment* comment = new (_commentPool.Alloc()) XMLComment( this );
    comment->_memPool = &_commentPool;
    comment->SetValue( str );
    return comment;
}


XMLText* MyXMLDocument::NewText( const char* str )
{
    XMLText* text = new (_textPool.Alloc()) XMLText( this );
    text->_memPool = &_textPool;
    text->SetValue( str );
    return text;
}


XMLDeclaration* MyXMLDocument::NewDeclaration( const char* str )
{
    XMLDeclaration* dec = new (_commentPool.Alloc()) XMLDeclaration( this );
    dec->_memPool = &_commentPool;
    dec->SetValue( str ? str : "xml version=\"1.0\" encoding=\"UTF-8\"" );
    return dec;
}


XMLUnknown* MyXMLDocument::NewUnknown( const char* str )
{
    XMLUnknown* unk = new (_commentPool.Alloc()) XMLUnknown( this );
    unk->_memPool = &_commentPool;
    unk->SetValue( str );
    return unk;
}


XMLError MyXMLDocument::LoadFile( const char* filename )
{
    Clear();
    FILE* fp = 0;

#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
    errno_t err = fopen_s(&fp, filename, "rb" );
    if ( !fp || err) {
#else
    fp = fopen( filename, "rb" );
    if ( !fp) {
#endif
        SetError( XML_ERROR_FILE_NOT_FOUND, filename, 0 );
        return _errorID;
    }
    LoadFile( fp );
    fclose( fp );
    return _errorID;
}


XMLError MyXMLDocument::LoadFile( FILE* fp )
{
    Clear();

    fseek( fp, 0, SEEK_END );
    size_t size = ftell( fp );
    fseek( fp, 0, SEEK_SET );

    if ( size == 0 ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }

    _charBuffer = new char[size+1];
    size_t read = fread( _charBuffer, 1, size, fp );
    if ( read != size ) {
        SetError( XML_ERROR_FILE_READ_ERROR, 0, 0 );
        return _errorID;
    }

    _charBuffer[size] = 0;

    const char* p = _charBuffer;
    p = XMLUtil::SkipWhiteSpace( p );
    p = XMLUtil::ReadBOM( p, &_writeBOM );
    if ( !p || !*p ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }

    ParseDeep( _charBuffer + (p-_charBuffer), 0 );
    return _errorID;
}


XMLError MyXMLDocument::SaveFile( const char* filename, bool compact )
{
    FILE* fp = 0;
#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
    errno_t err = fopen_s(&fp, filename, "w" );
    if ( !fp || err) {
#else
    fp = fopen( filename, "w" );
    if ( !fp) {
#endif
        SetError( XML_ERROR_FILE_COULD_NOT_BE_OPENED, filename, 0 );
        return _errorID;
    }
    SaveFile(fp, compact);
    fclose( fp );
    return _errorID;
}


XMLError MyXMLDocument::SaveFile( FILE* fp, bool compact )
{
    XMLPrinter stream( fp, compact );
    Print( &stream );
    return _errorID;
}


XMLError MyXMLDocument::Parse( const char* p, size_t len )
{
	const char* start = p;
    Clear();

    if ( !p || !*p ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }
    if ( len == (size_t)(-1) ) {
        len = strlen( p );
    }
    _charBuffer = new char[ len+1 ];
    memcpy( _charBuffer, p, len );
    _charBuffer[len] = 0;

    p = XMLUtil::SkipWhiteSpace( p );
    p = XMLUtil::ReadBOM( p, &_writeBOM );
    if ( !p || !*p ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }

    ptrdiff_t delta = p - start;	// skip initial whitespace, BOM, etc.
    ParseDeep( _charBuffer+delta, 0 );
    return _errorID;
}


void MyXMLDocument::Print( XMLPrinter* streamer ) const
{
    XMLPrinter stdStreamer( stdout );
    if ( !streamer ) {
        streamer = &stdStreamer;
    }
    Accept( streamer );
}


void MyXMLDocument::SetError( XMLError error, const char* str1, const char* str2 )
{
    _errorID = error;
    _errorStr1 = str1;
    _errorStr2 = str2;
}


void MyXMLDocument::PrintError() const
{
    if ( _errorID ) {
        static const int LEN = 20;
        char buf1[LEN] = { 0 };
        char buf2[LEN] = { 0 };

        if ( _errorStr1 ) {
            TIXML_SNPRINTF( buf1, LEN, "%s", _errorStr1 );
        }
        if ( _errorStr2 ) {
            TIXML_SNPRINTF( buf2, LEN, "%s", _errorStr2 );
        }

        printf( "MyXMLDocument error id=%d str1=%s str2=%s\n",
                _errorID, buf1, buf2 );
    }
}


XMLPrinter::XMLPrinter( FILE* file, bool compact, int depth ) :
    _elementJustOpened( false ),
    _firstElement( true ),
    _fp( file ),
    _depth( depth ),
    _textDepth( -1 ),
    _processEntities( true ),
    _compactMode( compact )
{
    for( int i=0; i<ENTITY_RANGE; ++i ) {
        _entityFlag[i] = false;
        _restrictedEntityFlag[i] = false;
    }
    for( int i=0; i<NUM_ENTITIES; ++i ) {
        TIXMLASSERT( entities[i].value < ENTITY_RANGE );
        if ( entities[i].value < ENTITY_RANGE ) {
            _entityFlag[ (int)entities[i].value ] = true;
        }
    }
    _restrictedEntityFlag[(int)'&'] = true;
    _restrictedEntityFlag[(int)'<'] = true;
    _restrictedEntityFlag[(int)'>'] = true;	// not required, but consistency is nice
    _buffer.Push( 0 );
}


void XMLPrinter::Print( const char* format, ... )
{
    va_list     va;
    va_start( va, format );

    if ( _fp ) {
        vfprintf( _fp, format, va );
    }
    else {
        // This seems brutally complex. Haven't figured out a better
        // way on windows.
#ifdef _MSC_VER
        int len = -1;
        int expand = 1000;
        while ( len < 0 ) {
            len = vsnprintf_s( _accumulator.Mem(), _accumulator.Capacity(), _TRUNCATE, format, va );
            if ( len < 0 ) {
                expand *= 3/2;
                _accumulator.PushArr( expand );
            }
        }
        char* p = _buffer.PushArr( len ) - 1;
        memcpy( p, _accumulator.Mem(), len+1 );
#else
        int len = vsnprintf( 0, 0, format, va );
        // Close out and re-start the va-args
        va_end( va );
        va_start( va, format );
        char* p = _buffer.PushArr( len ) - 1;
        vsnprintf( p, len+1, format, va );
#endif
    }
    va_end( va );
}


void XMLPrinter::PrintSpace( int depth )
{
    for( int i=0; i<depth; ++i ) {
        Print( "    " );
    }
}


void XMLPrinter::PrintString( const char* p, bool restricted )
{
    // Look for runs of bytes between entities to print.
    const char* q = p;
    const bool* flag = restricted ? _restrictedEntityFlag : _entityFlag;

    if ( _processEntities ) {
        while ( *q ) {
            // Remember, char is sometimes signed. (How many times has that bitten me?)
            if ( *q > 0 && *q < ENTITY_RANGE ) {
                // Check for entities. If one is found, flush
                // the stream up until the entity, write the
                // entity, and keep looking.
                if ( flag[(unsigned)(*q)] ) {
                    while ( p < q ) {
                        Print( "%c", *p );
                        ++p;
                    }
                    for( int i=0; i<NUM_ENTITIES; ++i ) {
                        if ( entities[i].value == *q ) {
                            Print( "&%s;", entities[i].pattern );
                            break;
                        }
                    }
                    ++p;
                }
            }
            ++q;
        }
    }
    // Flush the remaining string. This will be the entire
    // string if an entity wasn't found.
    if ( !_processEntities || (q-p > 0) ) {
        Print( "%s", p );
    }
}


void XMLPrinter::PushHeader( bool writeBOM, bool writeDec )
{
    if ( writeBOM ) {
        static const unsigned char bom[] = { TIXML_UTF_LEAD_0, TIXML_UTF_LEAD_1, TIXML_UTF_LEAD_2, 0 };
        Print( "%s", bom );
    }
    if ( writeDec ) {
        PushDeclaration( "xml version=\"1.0\"" );
    }
}


void XMLPrinter::OpenElement( const char* name )
{
    if ( _elementJustOpened ) {
        SealElement();
    }
    _stack.Push( name );

    if ( _textDepth < 0 && !_firstElement && !_compactMode ) {
        Print( "\n" );
    }
    if ( !_compactMode ) {
        PrintSpace( _depth );
    }

    Print( "<%s", name );
    _elementJustOpened = true;
    _firstElement = false;
    ++_depth;
}


void XMLPrinter::PushAttribute( const char* name, const char* value )
{
    TIXMLASSERT( _elementJustOpened );
    Print( " %s=\"", name );
    PrintString( value, false );
    Print( "\"" );
}


void XMLPrinter::PushAttribute( const char* name, int v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::PushAttribute( const char* name, unsigned v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::PushAttribute( const char* name, bool v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::PushAttribute( const char* name, double v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::CloseElement()
{
    --_depth;
    const char* name = _stack.Pop();

    if ( _elementJustOpened ) {
        Print( "/>" );
    }
    else {
        if ( _textDepth < 0 && !_compactMode) {
            Print( "\n" );
            PrintSpace( _depth );
        }
        Print( "</%s>", name );
    }

    if ( _textDepth == _depth ) {
        _textDepth = -1;
    }
    if ( _depth == 0 && !_compactMode) {
        Print( "\n" );
    }
    _elementJustOpened = false;
}


void XMLPrinter::SealElement()
{
    _elementJustOpened = false;
    Print( ">" );
}


void XMLPrinter::PushText( const char* text, bool cdata )
{
    _textDepth = _depth-1;

    if ( _elementJustOpened ) {
        SealElement();
    }
    if ( cdata ) {
        Print( "<![CDATA[" );
        Print( "%s", text );
        Print( "]]>" );
    }
    else {
        PrintString( text, true );
    }
}

void XMLPrinter::PushText( int value )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( unsigned value )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( bool value )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( float value )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( double value )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushComment( const char* comment )
{
    if ( _elementJustOpened ) {
        SealElement();
    }
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Print( "\n" );
        PrintSpace( _depth );
    }
    _firstElement = false;
    Print( "<!--%s-->", comment );
}


void XMLPrinter::PushDeclaration( const char* value )
{
    if ( _elementJustOpened ) {
        SealElement();
    }
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Print( "\n" );
        PrintSpace( _depth );
    }
    _firstElement = false;
    Print( "<?%s?>", value );
}


void XMLPrinter::PushUnknown( const char* value )
{
    if ( _elementJustOpened ) {
        SealElement();
    }
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Print( "\n" );
        PrintSpace( _depth );
    }
    _firstElement = false;
    Print( "<!%s>", value );
}


bool XMLPrinter::VisitEnter( const MyXMLDocument& doc )
{
    _processEntities = doc.ProcessEntities();
    if ( doc.HasBOM() ) {
        PushHeader( true, false );
    }
    return true;
}


bool XMLPrinter::VisitEnter( const XMLElement& element, const XMLAttribute* attribute )
{
    OpenElement( element.Name() );
    while ( attribute ) {
        PushAttribute( attribute->Name(), attribute->Value() );
        attribute = attribute->Next();
    }
    return true;
}


bool XMLPrinter::VisitExit( const XMLElement& )
{
    CloseElement();
    return true;
}


bool XMLPrinter::Visit( const XMLText& text )
{
    PushText( text.Value(), text.CData() );
    return true;
}


bool XMLPrinter::Visit( const XMLComment& comment )
{
    PushComment( comment.Value() );
    return true;
}

bool XMLPrinter::Visit( const XMLDeclaration& declaration )
{
    PushDeclaration( declaration.Value() );
    return true;
}


bool XMLPrinter::Visit( const XMLUnknown& unknown )
{
    PushUnknown( unknown.Value() );
    return true;
}

}   // namespace tinyxml2


//------------------------------------------------------------------------------------------------------
//	���캯��
//------------------------------------------------------------------------------------------------------
CMyFile_XML::CMyFile_XML(void)
{
	v_sDev_Name="";														//��Ԫ����
}

//------------------------------------------------------------------------------------------------------
//	��������
//------------------------------------------------------------------------------------------------------
CMyFile_XML::~CMyFile_XML(void)
{
	File_Reset();														//������λ
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�XML�ļ����ݶ�ȡ�������ļ���
//	��    ����v_sFile_Name���ļ����ƣ�
//	��    ����v_mObjType����
//	��    �أ�true����Ч�ļ���	false����Ч�ļ���
//------------------------------------------------------------------------------------------------------
bool CMyFile_XML::Read_XML(string v_sFile_Name)
{
	XMLElement* measCollecFile;											//ָ�룺measCollecFile
	XMLElement* measData;												//ָ�룺measData
	XMLElement* measInfo;												//ָ�룺measInfo
	XMLElement* measType=NULL;											//ָ�룺measType
	XMLElement* measValue=NULL;											//ָ�룺measValue
	XMLElement* r=NULL;													//ָ�룺r

	mString_String	v_mCounter;											//ʵ������counter	[����map]
	list_Object	v_lObject;												//ʵ������Object	[����]
	File_Object v_cObject;												//ʵ������Object	[�ṹ]
	CString		v_sType_Name,v_sR_Value;								//��ʱ����
	CString		v_sObject,v_sObject_DN;									//��ʱ����
	mString_String v_mName;												//��ʱ����

	//���������Ϊ��ȡ�ļ�����׼��
	File_Reset();														//������λ

	//�ļ���ȡ
	MyXMLDocument* doc = new MyXMLDocument();							//XMLʵ����
	doc->LoadFile(v_sFile_Name.c_str());								//�ļ���ȡ	[ȫ����ȡ���ڴ���]

	//��ã���Ԫ����
	measCollecFile= doc->FirstChildElement("NrmFile");					//��ѯ��NrmFile 
	if (measCollecFile==NULL)
		return  false;

	//��ȡ��ObjType\Objject
	measData= measCollecFile->FirstChildElement("Objects");				//��ѯ��Objects
	if (measData==NULL)
		return  false;
	measInfo=measData->FirstChildElement("ObjectType");					//��ѯ��ObjectType
	if (measData==NULL)
		return  false;
	v_sDev_Name=measInfo->GetText();									//�ļ���Ϣ

	//��ã�FieldName
	measInfo = measInfo->NextSiblingElement("FieldName");				//��ѯ��FieldName
	if(measInfo==NULL)
		return false;
	r=measInfo->FirstChildElement("N");									//��ѯ��N
	while(r!=NULL)														//
	{
		v_sType_Name=r->Attribute("i");
		v_sR_Value=r->GetText();										//��ã�Counterֵ
		v_mName.insert(pair<CString,CString> (v_sType_Name,v_sR_Value));	//����	[������v_mName]
		r=r->NextSiblingElement("N");									//��ã���һ��r
	}

	measInfo = measInfo->NextSiblingElement("FieldValue");				//��ѯ��FieldValue
	if(measInfo==NULL)
		return false;
	measValue=measInfo->FirstChildElement("Cm");						//��ѯ��Cm
	while(measValue!=NULL)
	{
		v_sObject=measValue->Attribute("UserLabel");					//DM����
		ConvertUtf8ToGBK(v_sObject);									//��ʽת��
		v_sObject_DN=measValue->Attribute("Dn");						//DM����
		r=measValue->FirstChildElement("V");							//��ѯ��V
		while(r!=NULL)													//
		{
			v_sType_Name=r->Attribute("i");
			v_sR_Value=r->GetText();									//��ã�Counterֵ
			ConvertUtf8ToGBK(v_sR_Value);								//��ʽת��
			mString_String::iterator v_pName=v_mName.find(v_sType_Name);//��������
			if (v_pName!=v_mName.end())
				v_mCounter.insert(pair<CString,CString> (v_pName->second,v_sR_Value));	//����	[������v_mCounter]
			r=r->NextSiblingElement("V");								//��ã���һ��r
		}
		v_cObject.v_mCounter = v_mCounter;								//Counter
		v_cObject.v_sObject_ID =v_sObject;								//Object
		v_cObject.v_sObject_DN=v_sObject_DN;							//Object_DN
		v_lObject.push_back(v_cObject);									//����Object
		v_mCounter.clear();												//��λ��v_mCounter [������Counter]
		measValue=measValue->NextSiblingElement("Cm");					//��ã���һ��Cm
	}
	v_mObjType.insert(pair<CString,list_Object> (v_sDev_Name,v_lObject));//���ӣ�[������v_mObjType]
	
	//����ڴ�ռ�
	list_Object::iterator v_pIterator_Object=v_lObject.begin();	
	while(v_pIterator_Object!=v_lObject.end())
	{
		v_pIterator_Object->v_mCounter.clear();							//Counter���������
		v_pIterator_Object++;
	}
	v_lObject.clear();													//��λ��v_lObject [����Object]
	v_mName.clear();													//��λ��v_mName [������v_mName]

	delete doc;
	doc = NULL;

	return true;														//��Ч����
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�XML�ļ����ݶ�ȡ�������ļ���
//	��    ����v_sFile_Name���ļ����ƣ�
//	��    ����v_mObjType����
//	��    �أ�true����Ч�ļ���	false����Ч�ļ���
//------------------------------------------------------------------------------------------------------
bool CMyFile_XML::Read_XML_PM(string v_sFile_Name)
{
	XMLElement* measCollecFile;											//ָ�룺measCollecFile
	XMLElement* measData;												//ָ�룺measData
	XMLElement* measInfo;												//ָ�룺measInfo
	XMLElement* measType=NULL;											//ָ�룺measType
	XMLElement* measValue=NULL;											//ָ�룺measValue
	XMLElement* r=NULL;													//ָ�룺r

	mString_String	v_mCounter;											//ʵ������counter	[����map]
	list_Object	v_lObject;												//ʵ������Object	[����]
	File_Object v_cObject;												//ʵ������Object	[�ṹ]
	CString		v_sType_Name,v_sR_Value,v_sTemp;						//��ʱ����
	CString		v_sObject,v_sObject_DN;									//��ʱ����
	mString_String v_mName;												//��ʱ����
	int			v_iNum=1;												//��ʱ����

	//���������Ϊ��ȡ�ļ�����׼��
	File_Reset();														//������λ

	//�ļ���ȡ
	MyXMLDocument* doc = new MyXMLDocument();							//XMLʵ����
	doc->LoadFile(v_sFile_Name.c_str());								//�ļ���ȡ	[ȫ����ȡ���ڴ���]

	//��ã���Ԫ����
	measCollecFile= doc->FirstChildElement("PmFile");					//��ѯ��PmFile 
	if (measCollecFile==NULL)
		return  false;

	//��ȡ��ObjType\Objject
	measData= measCollecFile->FirstChildElement("Measurements");		//��ѯ��Measurements
	while(measData!=NULL)
	{
		measInfo=measData->FirstChildElement("ObjectType");					//��ѯ��ObjectType
		if (measData==NULL)
			return  false;
		v_sDev_Name=measInfo->GetText();									//�ļ���Ϣ

		//��ã�FieldName
		measInfo = measInfo->NextSiblingElement("PmName");					//��ѯ��PmName
		if(measInfo==NULL)
			return false;
		r=measInfo->FirstChildElement("N");									//��ѯ��N
		while(r!=NULL)														//
		{
			v_sType_Name=r->Attribute("i");
			v_sR_Value=r->GetText();										//��ã�Counterֵ
			v_mName.insert(pair<CString,CString>(v_sType_Name,v_sR_Value));	//����	[������v_mName]
			r=r->NextSiblingElement("N");									//��ã���һ��r
		}

		measInfo = measInfo->NextSiblingElement("PmData");					//��ѯ��PmData
		if(measInfo==NULL)
			return false;
		measValue=measInfo->FirstChildElement("Pm");						//��ѯ��Pm
		while(measValue!=NULL)
		{
			v_sObject=measValue->Attribute("UserLabel");					//DM����
			ConvertUtf8ToGBK(v_sObject);									//��ʽת��
			v_sObject_DN=measValue->Attribute("Dn");						//DM����
			r=measValue->FirstChildElement("V");							//��ѯ��V
			while(r!=NULL)													//
			{
				v_sType_Name=r->Attribute("i");
				v_sR_Value=r->GetText();									//��ã�Counterֵ
				ConvertUtf8ToGBK(v_sR_Value);								//��ʽת��
				mString_String::iterator v_pName=v_mName.find(v_sType_Name);//��������
				if (v_pName!=v_mName.end())
					v_mCounter.insert(pair<CString,CString> (v_pName->second,v_sR_Value));	//����	[������v_mCounter]
				r=r->NextSiblingElement("V");								//��ã���һ��r
			}
			v_cObject.v_mCounter = v_mCounter;								//Counter
			v_cObject.v_sObject_ID =v_sObject;								//Object
			v_cObject.v_sObject_DN=v_sObject_DN;							//Object_DN
			v_lObject.push_back(v_cObject);									//����Object
			v_mCounter.clear();												//��λ��v_mCounter [������Counter]
			measValue=measValue->NextSiblingElement("Pm");					//��ã���һ��Pm
		}
		v_sTemp.Format("%02d",v_iNum);										//�ַ���
		v_mObjType.insert(pair<CString,list_Object> (v_sTemp+"_"+v_sDev_Name,v_lObject));	//���ӣ�[������v_mObjType]

		v_iNum++;														//���
		//����ڴ�ռ�
		list_Object::iterator v_pIterator_Object=v_lObject.begin();	
		while(v_pIterator_Object!=v_lObject.end())
		{
			v_pIterator_Object->v_mCounter.clear();						//Counter���������
			v_pIterator_Object++;
		}
		v_lObject.clear();												//��λ��v_lObject [����Object]
		v_mName.clear();												//��λ��v_mName [������v_mName]
		measData=measData->NextSiblingElement("Measurements");			//��ã���һ��Measurements
	}

	delete doc;
	doc = NULL;

	return true;														//��Ч����
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�XML�ļ����ݶ�ȡ�������ļ���ֱ�Ӷ�ȡ��
//	��    ����v_sFile_Name���ļ����ƣ�
//	��    ����v_mObjType����
//	��    �أ�true����Ч�ļ���	false����Ч�ļ���
//------------------------------------------------------------------------------------------------------
bool CMyFile_XML::Read_XML_PM_Dir(CString v_sFile_Name)
{
	mString_String	v_mCounter;											//ʵ������counter	[����map]
	File_Object		v_cObject;											//ʵ������Object	[�ṹ]
	CString			v_sTemp;											//��ʱ����
	int				v_iNum=0;											//��ʱ����

	//���������Ϊ��ȡ�ļ�����׼��
	File_Reset();														//������λ

	//�ļ���ȡ
	if(!Read_XML_PM_Open(v_sFile_Name))									//���ļ�
		return false;													//����

	//��ȡ��ObjType\Objject
	while(Read_XML_PM_Element("<Measurements>"))						//��ѯ��Measurements
	{
		v_sDev_Name=Read_XML_PM_GetText("ObjectType");					//�ļ���Ϣ

		//��ã�FieldName
		v_sType_Name="";												//�����
		v_sR_Value="";													//������
		Read_XML_PM_Element("<PmName>");								//��ѯ����Ԫ�ء�PmName��
		v_sElement_Key="</PmName>";										//��ȡ����Ԫ��
		while(Read_XML_PM_Col())										//��ã�����Ϣ
		{
			v_mName.insert(pair<CString,CString>(v_sType_Name,v_sR_Value));	//����	[������v_mName]
		}

		//��ȡ����Ԫ��Ϣ
		Read_XML_PM_Element("<PmData>");								//��ѯ����Ԫ�ء�PmName��
		while(Read_XML_PM_GetKey("Pm"))									//��ã�������
		{
			v_sElement_Key="</Pm>";										//��ȡ����Ԫ��
			while(Read_XML_PM_Row())									//��ã�����Ϣ
			{
				if (!v_sType_Name.IsEmpty())
				{
					mString_String::iterator v_pName=v_mName.find(v_sType_Name);				//��������
					if (v_pName!=v_mName.end())
						v_mCounter.insert(pair<CString,CString> (v_pName->second,v_sR_Value));	//����	[������v_mCounter]
				}
			}
			v_cObject.v_mCounter = v_mCounter;							//Counter
			v_cObject.v_sObject_ID=v_sObject;							//Object
			v_cObject.v_sObject_DN=v_sObject_DN;						//Object_DN

			//�޳�����˳�е�С���������ļ��������������˳С����Ϣ�������⣺��������С���Ը�˳��ͷ����δ���������
			if (v_sObject.Left(4)!="��˳")								//Ϊ��˳С��������							
				v_cObject_Child.v_lObject.push_back(v_cObject);			//����Object

			v_mCounter.clear();											//��λ��v_mCounter [������Counter]
		}
		v_iNum++;
		v_sTemp.Format("%d",v_iNum);									//���
		v_mObjType_Child.insert(pair<CString,File_Object_Child> (v_sTemp+"_"+v_sDev_Name,v_cObject_Child));	//���ӣ���������v_mObjType_Child��

		//����ڴ�ռ�
		//�����v_lObject
		list_Object::iterator v_pIterator_Object=v_cObject_Child.v_lObject.begin();	
		while(v_pIterator_Object!=v_cObject_Child.v_lObject.end())
		{
			v_pIterator_Object->v_mCounter.clear();						//Counter���������
			v_pIterator_Object++;
		}
		v_cObject_Child.v_lObject.clear();								//��λ��v_lObject [����Object]

		//�����v_mChild
		map_Child::iterator			v_pChild;							//��������map_Child
		map_C_Data::iterator		v_pChild_Data;						//��������map_Child_Data
		v_pChild=v_cObject_Child.v_mChild.begin();
		while(v_pChild!=v_cObject_Child.v_mChild.end())
		{
			if (v_pChild->second.size()!=0)
			{
				v_pChild_Data=v_pChild->second.begin();
				while(v_pChild_Data!=v_pChild->second.end())
				{
					v_pChild_Data->second.clear();						//���������v_mCounter��Counter��
					v_pChild_Data++;									//������++
				}
				v_pChild->second.clear();								//���������v_mChild_Data��Cell��
			}
			v_pChild++;													//������++
		}
		v_cObject_Child.v_mChild.clear();								//���������v_mChild�����
	}

	v_fRead.Close();													//�ر��ļ�
	return true;														//��Ч����
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ����ļ�
//	��    ����v_sFile_Name���ļ����ƣ�
//	��    �أ�true����Ч�ļ���	false����Ч�ļ���
//------------------------------------------------------------------------------------------------------
bool CMyFile_XML::Read_XML_PM_Open(CString v_sFile_Name)
{
	return v_fRead.Open(_T(v_sFile_Name),CFile::modeRead);				//���ļ���������
}
	
//------------------------------------------------------------------------------------------------------
//	�������ܣ����ҡ���Ԫ�ء�
//	��    ����v_sElement��Ԫ�����ƣ�
//	��    �أ�true���ҵ���	false��û�ҵ���
//------------------------------------------------------------------------------------------------------
bool CMyFile_XML::Read_XML_PM_Element(CString v_sElement)
{
	CString		v_sStr;													//��ʱ����

	while(v_fRead.ReadString(v_sStr))									//���ݶ�ȡ��һ��
	{
		int	v_iLen=v_sStr.Find(v_sElement,0);							//���ң�Ԫ��
		if (v_iLen>=0)													//�ҵ�������
			return true;												//���أ��ҵ�
	}
	return false;														//���أ�û�ҵ�
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ���ȡ��Ԫ��ֵ��
//	��    ����v_sElement_Key��Ԫ�����ƣ�
//	��    �أ�true���ҵ���	false��û�ҵ���
//------------------------------------------------------------------------------------------------------
CString CMyFile_XML::Read_XML_PM_GetText(CString v_sElement)
{
	CString		v_sStr;													//��ʱ����
	int			v_iLen;													//��ʱ����

	v_sElement="<"+v_sElement+">";										//����
	while(v_fRead.ReadString(v_sStr))									//���ݶ�ȡ��һ��
	{
		v_iLen=v_sStr.Find(v_sElement,0);								//���ң�Ԫ��
		if (v_iLen>=0)													//�ҵ�������
		{
			v_sStr=v_sStr.Mid(v_iLen+v_sElement.GetLength());			//��ȡ
			v_iLen=v_sStr.Find("<",0);									//���ң�"<"
			if (v_iLen>=0)												//�ҵ�������
				v_sStr=v_sStr.Left(v_iLen);								//��ȡ
			return v_sStr;												//���أ�Ԫ��ֵ
		}
	}
	return "";															//���أ�û�ҵ�
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ���ã�������
//	����������v_sObject����������	v_sObject_DN��Dn��
//	��    ����v_sElement��Ԫ�����ơ���ѯ��
//	��    �أ�true���ҵ���	false��û�ҵ���
//------------------------------------------------------------------------------------------------------
bool CMyFile_XML::Read_XML_PM_GetKey(CString v_sElement)
{
	CString		v_sStr;													//��ʱ����
	int			v_iLen;													//��ʱ����

	//���ң�v_sElement
	v_sElement="<"+v_sElement;											//����
	while(v_fRead.ReadString(v_sStr))									//���ݶ�ȡ��һ��
	{
		//����������
		v_iLen=v_sStr.Find("</PmData>",0);								//���ң���Ԫ�أ�����
		if (v_iLen>=0)													//�ҵ�������
			return false;												//������������
		v_iLen=v_sStr.Find(v_sElement,0);								//���ң�Ԫ��
		if (v_iLen>=0)													//�ҵ�������
		{
			//���ң�Dn
			v_sObject_DN="";											//Ĭ��
			v_iLen=v_sStr.Find("Dn=\"",0);								//���ң�Dn
			if (v_iLen>=0)												//�ҵ�������
			{
				v_sStr=v_sStr.Mid(v_iLen+4);							//��ȡ
				v_iLen=v_sStr.Find("\"",0);								//���ң�"""
				if (v_iLen>=0)											//�ҵ�������
					v_sObject_DN=v_sStr.Left(v_iLen);					//��ȡ
			}
			//���ң�������
			v_sObject="";												//Ĭ��
			v_iLen=v_sStr.Find("UserLabel=\"",0);						//���ң�Dn
			if (v_iLen>=0)												//�ҵ�������
			{
				v_sStr=v_sStr.Mid(v_iLen+11);							//��ȡ
				v_iLen=v_sStr.Find("\"",0);								//���ң�"""
				if (v_iLen>=0)											//�ҵ�������
				{
					v_sObject=v_sStr.Left(v_iLen);						//��ȡ
					ConvertUtf8ToGBK(v_sObject);						//��ʽת��
				}
			}
			return true;												//���أ���ȡ
		}
	}
	return false;														//���أ�û�ҵ�
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ���ȡ������Ϣ��
//	����������v_sType_Name������ţ�	v_sR_Value�������ƣ�
//	����������v_sElement��Ԫ�����ơ�������ѯ��
//	��    �أ�true���ҵ���	false��û�ҵ���
//------------------------------------------------------------------------------------------------------
bool CMyFile_XML::Read_XML_PM_Col()
{
	CString		v_sStr;													//��ʱ����
	int			v_iLen;													//��ʱ����

	if(v_fRead.ReadString(v_sStr))										//���ݶ�ȡ��һ��
	{
		//����������
		v_iLen=v_sStr.Find(v_sElement_Key,0);							//���ң���Ԫ�أ�����
		if (v_iLen>=0)													//�ҵ�������
			return false;												//������������
		//��ȡ��������<N i="1">PHY.CellMaxTxPower</N>��
		v_iLen=v_sStr.Find("=\"",0);									//���ң�"=""
		if (v_iLen>=0)													//�ҵ�������
		{
			v_sStr=v_sStr.Mid(v_iLen+2);								//��ȡ
			v_iLen=v_sStr.Find("\"",0);									//���ң�"""
			if (v_iLen>=0)												//�ҵ�������
			{
				v_sType_Name=v_sStr.Left(v_iLen);						//��ȡ�������
				v_iLen=v_sStr.Find(">",0);								//���ң�">"
				if (v_iLen>=0)											//�ҵ�������
				{
					v_sStr=v_sStr.Mid(v_iLen+1);						//��ȡ
					v_iLen=v_sStr.Find("</N>",0);						//���ң�"</N>"
					if (v_iLen>=0)										//�ҵ�������
					{
						v_sR_Value=v_sStr.Left(v_iLen);					//��ȡ�������
						return true;
					}
				}
			}
		}
	}
	return false;														//���أ�û�ҵ�
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ���ȡ������Ϣ��
//	����������v_sType_Name������ţ�	v_sR_Value����Ԫ��ֵ��
//	����������v_sElement_Key��Ԫ�����ơ�������ѯ��
//	��    �أ�true���ҵ���	false��û�ҵ���
//	��    ����(1)ȡ������	<V i="1">3.72</V>�����ã�3.72
//			  (2)��ͣ���	<CV i="7">
//							<SN>CONTEXT.AttRelEnb.HOFailure</SN>
//							<SV>1</SV>
//							<SN>CONTEXT.AttRelEnb.RNL</SN>
//							<SV>2</SV>
//							<SN>CONTEXT.AttRelEnb.UeLost</SN>
//							<SV>3</SV>		
//							</CV>			��
//					�ã�6
//------------------------------------------------------------------------------------------------------
bool CMyFile_XML::Read_XML_PM_Row()
{
	CString		v_sStr;													//��ʱ����
	int			v_iLen;													//��ʱ����

	if(v_fRead.ReadString(v_sStr))										//���ݶ�ȡ��һ��
	{
		//����������
		v_iLen=v_sStr.Find(v_sElement_Key,0);							//���ң���Ԫ�أ�����
		if (v_iLen>=0)													//�ҵ�������
			return false;												//������������
		//��ȡ��������<V i="1">3.72</V>��
		v_iLen=v_sStr.Find("=\"",0);									//���ң���="��
		if (v_iLen>=0)													//�ҵ�������
		{
			v_sStr=v_sStr.Mid(v_iLen+2);								//��ȡ
			v_iLen=v_sStr.Find("\"",0);									//���ң���"��
			if (v_iLen>=0)												//�ҵ�������
			{
				v_sType_Name=v_sStr.Left(v_iLen);						//��ȡ�������
				v_sStr=v_sStr.Mid(v_iLen);								//������">3.72</V> �� "> ��
				if (v_sStr.GetLength()>3)								//��ȡ��ֵ
				{
					v_iLen=v_sStr.Find(">",0);							//���ң�">"
					if (v_iLen>=0)										//�ҵ�������
					{
						v_sStr=v_sStr.Mid(v_iLen+1);					//��ȡ
						v_iLen=v_sStr.Find("</V>",0);					//���ң�"</V>"
						if (v_iLen>=0)									//�ҵ�������
						{
							v_sR_Value=v_sStr.Left(v_iLen);				//��ȡ��ֵ
							return true;
						}
					}
				}
				else													//������͡����
				{
					v_sR_Value=Read_XML_PM_Row_Add("</CV>");			//��ͣ�ֵ
					return true;
				}
			}
		}
		v_sType_Name="";												//��Ч����
		return true;													//���أ�û�ҵ�
	}
	return false;														//���أ�û�ҵ�
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�������͡����������Child������
//	����������v_sEnd����������
//	��    ������ͣ���	<CV i="7">
//							<SN>CONTEXT.AttRelEnb.HOFailure</SN>
//							<SV>1</SV>
//							<SN>CONTEXT.AttRelEnb.RNL</SN>
//							<SV>2</SV>
//							<SN>CONTEXT.AttRelEnb.UeLost</SN>
//							<SV>3</SV>		
//							</CV>			��
//					�ã�6
//------------------------------------------------------------------------------------------------------
CString CMyFile_XML::Read_XML_PM_Row_Add(CString v_sEnd)
{
	CString		v_sStr,v_sKey,v_sValue;									//��ʱ����
	int			v_iLen;													//��ʱ����
	double		v_dSum=0;												//��ʱ����

	map_C_Data					v_mChild_Data;							//������map_Child_Data
	map_C_Data::iterator		v_pChild_Data;							//��������map_Child_Data
	mString_String				v_mCounter;								//������mString_String
	map_Child::iterator			v_pChild;								//��������v_mChild


	bool	v_bOK=false;												//������Ч��־
	if (!v_sType_Name.IsEmpty())										//��Ч��Counter��ţ�������2��
	{
		mString_String::iterator v_pName = v_mName.find(v_sType_Name);	//���ң�Counter��š�2��
		if (v_pName!=v_mName.end())										//�ҵ�������
		{
			v_pChild = v_cObject_Child.v_mChild.find(v_pName->second);	//���ң������ƣ�����
			if (v_pChild==v_cObject_Child.v_mChild.end())				//û�ҵ�������
			{
				v_cObject_Child.v_mChild.insert(pair<CString,map_C_Data>(v_pName->second,v_mChild_Data));	//��������
				v_pChild = v_cObject_Child.v_mChild.find(v_pName->second);		//���ң������ƣ�����
			}
			if (v_sObject.Left(4)!="��˳")								//Ϊ��˳С��������
			{
				v_pChild_Data=v_pChild->second.find(v_sObject);			//С�����ڣ�����
				if (v_pChild_Data==v_pChild->second.end())				//û�ҵ�������
				{
					v_pChild->second.insert(pair<CString,mString_String>(v_sObject,v_mCounter));	//�������ӣ�С��
					v_pChild_Data=v_pChild->second.find(v_sObject);		//����
				}
				v_bOK=true;												//��λ��������Ч
			}
		}
	}

	while(v_fRead.ReadString(v_sStr))									//���ݶ�ȡ��һ��
	{
		//����������
		v_iLen=v_sStr.Find(v_sEnd,0);									//���ң���Ԫ�أ�����
		if (v_iLen>=0)													//�ҵ�������
			break;														//�˳�
		//��ȡ�����������<SN>CONTEXT.AttRelEnb.RNL</SN>��
		v_iLen=v_sStr.Find("<SN>",0);									//���ң�"<SN>"
		if (v_iLen>=0)													//�ҵ�������
		{
			v_sStr=v_sStr.Mid(v_iLen+4);								//��ȡ
			v_iLen=v_sStr.Find("</SN>",0);								//���ң�"</SN>"
			if (v_iLen>=0)												//�ҵ�������
				v_sKey = v_sStr.Left(v_iLen);							//��ȡ������������
		}
		else
		{																//��ȡ����ֵ��<SV>2</SV>��
			v_iLen=v_sStr.Find("<SV>",0);								//���ң�"<SV>"
			if (v_iLen>=0)												//�ҵ�������
			{
				v_sStr=v_sStr.Mid(v_iLen+4);							//��ȡ
				v_iLen=v_sStr.Find("</SV>",0);							//���ң�"</SV>"
				if (v_iLen>=0)											//�ҵ�������
				{
					v_sValue=v_sStr.Left(v_iLen);						//��ȡ����ֵ
					v_dSum+=atof(v_sValue);								//���
					if (!v_sKey.IsEmpty())								//������Ч������
					{													//��������������v_mChild��Counter��map(�����ֵ)��
						if (!v_sType_Name.IsEmpty() && v_bOK)			//��Ч��Counter��ţ�������2��
						{
							v_pChild_Data->second.insert(pair<CString,CString>(v_sKey,v_sValue));	//����������(�����У�����)
						}
					}
				}
			}
		}
	}
	v_sStr.Format("%10.6f",v_dSum);

	return v_sStr;														//���أ���
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ�XML�ļ����ݶ�ȡ���澯�ļ���
//	��    ����v_sFile_Name���ļ����ƣ�
//	��    ����v_mObjType����
//	��    �أ�true����Ч�ļ���	false����Ч�ļ���
//------------------------------------------------------------------------------------------------------
bool CMyFile_XML::Read_XML_Alarm(string v_sFile_Name)
{
	XMLElement* measCollecFile;											//ָ�룺measCollecFile
	XMLElement* measData;												//ָ�룺measData
	XMLElement* measInfo;												//ָ�룺measInfo
	XMLElement* measValue=NULL;											//ָ�룺measValue
	XMLElement* r=NULL;													//ָ�룺r

	mString_String	v_mCounter;											//ʵ������counter	[����map]
	list_Object	v_lObject;												//ʵ������Object	[����]
	File_Object v_cObject;												//ʵ������Object	[�ṹ]
	CString		v_sType_Name,v_sR_Value;								//��ʱ����
	CString		v_sObject,v_sObject_DN;									//��ʱ����
	mString_String v_mName;												//��ʱ����

	//	v_mName.insert(pair<CString,CString> (v_sType_Name,v_sR_Value));	//����	[������v_mName]
	//���������Ϊ��ȡ�ļ�����׼��
	File_Reset();														//������λ

	//�ļ���ȡ
	MyXMLDocument* doc = new MyXMLDocument();							//XMLʵ����
	doc->LoadFile(v_sFile_Name.c_str());								//�ļ���ȡ	[ȫ����ȡ���ڴ���]

	//��ã���Ԫ����
	measCollecFile= doc->FirstChildElement("alarmSynFile");				//��ѯ��alarmSynFile 
	if (measCollecFile==NULL)
		return  false;

	//��ȡ��ObjType\Objject
	measData= measCollecFile->FirstChildElement("fileHeader");			//��ѯ��fileHeader 
	if (measData==NULL)
		return  false;
	measData=measData->NextSiblingElement("alarmList");					//��ѯ��alarmList���澯�б�
	if (measData==NULL)
		return  false;
	
	//��ѯ����
	v_sDev_Name="AlarmList";											//��ѯ����
	//������
	v_mName.insert(pair<CString,CString> ("objectClass",""));			//����	[������v_mName]
	v_mName.insert(pair<CString,CString> ("objectInstance",""));
	v_mName.insert(pair<CString,CString> ("notificationId",""));
	v_mName.insert(pair<CString,CString> ("eventTime",""));
	v_mName.insert(pair<CString,CString> ("alarmClearedTime",""));
	v_mName.insert(pair<CString,CString> ("systemDN",""));
	v_mName.insert(pair<CString,CString> ("vendorSpecificAlarmType",""));
	v_mName.insert(pair<CString,CString> ("notificationType",""));
	v_mName.insert(pair<CString,CString> ("alarmId",""));
	v_mName.insert(pair<CString,CString> ("alarmRaisedTime",""));
	v_mName.insert(pair<CString,CString> ("alarmType",""));
	v_mName.insert(pair<CString,CString> ("probableCause",""));
	v_mName.insert(pair<CString,CString> ("perceivedSeverity",""));
	v_mName.insert(pair<CString,CString> ("specificProblem",""));
	v_mName.insert(pair<CString,CString> ("backedUpStatus",""));
	v_mName.insert(pair<CString,CString> ("trendIndication",""));
	v_mName.insert(pair<CString,CString> ("additionalText",""));
	v_mName.insert(pair<CString,CString> ("additionalInformation_ps",""));
	v_mName.insert(pair<CString,CString> ("additionalInformation_at",""));
	v_mName.insert(pair<CString,CString> ("additionalInformation_ai_vs_other",""));

	measInfo=measData->FirstChildElement("alarmInfo");					//��ѯ��alarmInfo
	while(measInfo!=NULL)
	{
		mString_String::iterator v_pName=v_mName.begin();				//��������
		while(v_pName!=v_mName.end())
		{
			measValue=measInfo->FirstChildElement(v_pName->first);		//���ң�������
			if (measValue!=NULL)
				v_sR_Value=measValue->GetText();						//��ã�Counterֵ
			else
				v_sR_Value="";											//������Ϊ��
			ConvertUtf8ToGBK(v_sR_Value);								//��ʽת��
			v_mCounter.insert(pair<CString,CString> (v_pName->first,v_sR_Value));	//����	[������v_mCounter]
			v_pName++;													
		}
		measInfo=measInfo->NextSiblingElement("alarmInfo");				//��ã���һ��alarmInfo
		v_cObject.v_mCounter = v_mCounter;								//Counter
		v_mCounter.clear();												//��λ��v_mCounter [������Counter]
		v_lObject.push_back(v_cObject);									//����Object
	}
	v_mObjType.insert(pair<CString,list_Object> (v_sDev_Name,v_lObject));//���ӣ�[������v_mObjType]

	//����ڴ�ռ�
	list_Object::iterator v_pIterator_Object=v_lObject.begin();	
	while(v_pIterator_Object!=v_lObject.end())
	{
		v_pIterator_Object->v_mCounter.clear();							//Counter���������
		v_pIterator_Object++;
	}
	v_lObject.clear();													//��λ��v_lObject [����Object]
	v_mName.clear();													//��λ��v_mName [������v_mName]

	delete doc;
	doc = NULL;

	return true;														//��Ч����
}

//------------------------------------------------------------------------------------------------------
//	�������ܣ���ʽת�� [Utf8ToGBK]
//------------------------------------------------------------------------------------------------------
void CMyFile_XML::ConvertUtf8ToGBK( CString& strUtf8 )   
{   
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);   
	WCHAR * wszGBK = new WCHAR[len+1];   
	memset(wszGBK, 0, len * 2 + 2);   
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, wszGBK, len);   
	  
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);   
	char *szGBK=new char[len + 1];   
	memset(szGBK, 0, len + 1);   
	WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);   
	  
	strUtf8 = szGBK;   
	delete[] szGBK;   
	delete[] wszGBK;   
}   

//------------------------------------------------------------------------------------------------------
//	�������ܣ�������λ
//------------------------------------------------------------------------------------------------------
void CMyFile_XML::File_Reset()
{
	v_sDev_Name="";														//��Ԫ���Ƹ�λ

	//������λ��Objtype�����á����ܡ�
	map_ObjType::iterator	v_pIterator_ObjType;						//list_ObjType������
	list_Object::iterator	v_pIterator_Object;							//list_Object������
	v_pIterator_ObjType=v_mObjType.begin();
	while(v_pIterator_ObjType!=v_mObjType.end())
	{
		v_pIterator_Object=v_pIterator_ObjType->second.begin();
		while(v_pIterator_Object!=v_pIterator_ObjType->second.end())
		{
			v_pIterator_Object->v_mCounter.clear();						//Counter���������
			v_pIterator_Object++;
		}
		v_pIterator_ObjType->second.clear();							//Object�������
		v_pIterator_ObjType++;
	}
	v_mObjType.clear();													//ObjType���������

	//������λ��map_ObjType_Child������ֱ����
	map_ObjType_Child::iterator	v_pObjType_Child;						//��������v_mObjType_Child
	map_Child::iterator			v_pChild;								//��������map_Child
	map_C_Data::iterator	v_pChild_Data;								//��������map_Child_Data
	v_pObjType_Child=v_mObjType_Child.begin();
	while(v_pObjType_Child!=v_mObjType_Child.end())
	{
		//�����v_lObject
		v_pIterator_Object=v_pObjType_Child->second.v_lObject.begin();
		while(v_pIterator_Object!=v_pObjType_Child->second.v_lObject.end())
		{
			v_pIterator_Object->v_mCounter.clear();						//���������v_mCounter��Counter��
			v_pIterator_Object++;										//������++
		}
		v_pObjType_Child->second.v_lObject.clear();						//���������v_lObject


		//�����v_mChild
		v_pChild=v_pObjType_Child->second.v_mChild.begin();
		while(v_pChild!=v_pObjType_Child->second.v_mChild.end())
		{
			if (v_pChild->second.size()!=0)
			{
				v_pChild_Data=v_pChild->second.begin();
				while(v_pChild_Data!=v_pChild->second.end())
				{
					v_pChild_Data->second.clear();						//���������v_mCounter��Counter��
					v_pChild_Data++;									//������++
				}
				v_pChild->second.clear();								//���������v_mChild_Data��Cell��
			}
			v_pChild++;													//������++
		}
		v_pObjType_Child->second.v_mChild.clear();						//���������v_mChild�����

		v_pObjType_Child++;
	}
	v_mObjType_Child.clear();											//v_mObjType_Child���������

	//������λ��������
	v_mName.clear();													//��λ��v_mName [������v_mName]
}

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
