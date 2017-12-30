#include "..\CheatInclude.h"

HFont Font::MediumESP;
HFont Font::TinyESP;
HFont Font::ESP;
HFont Font::LBY;

void D::SetupFonts()
{
	g_pSurface->SetFontGlyphSet( Font::MediumESP = g_pSurface->Create_Font(), charenc( "Verdana" ), 10, FW_BOLD, NULL, 20, FONTFLAG_OUTLINE, FONTFLAG_DROPSHADOW);
	g_pSurface->SetFontGlyphSet( Font::TinyESP = g_pSurface->Create_Font(), charenc( "Verdana" ), 10, FW_BOLD, NULL, 20, FONTFLAG_OUTLINE, FONTFLAG_DROPSHADOW);
	g_pSurface->SetFontGlyphSet( Font::ESP = g_pSurface->Create_Font(), charenc( "Verdana" ), 12, FW_BOLD, NULL, 20, FONTFLAG_OUTLINE, FONTFLAG_DROPSHADOW);
	//b1g ("Verdana", 12, FONT_CREATE_SPRITE | FONT_CREATE_OUTLINE | FONT_CREATE_BOLD);
	//b1g 2 ? xd ("Verdana", 12, 800, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	g_pSurface->SetFontGlyphSet( Font::LBY = g_pSurface->Create_Font(), charenc( "04b03" ), 36, FW_EXTRABOLD, NULL, NULL, FONTFLAG_DROPSHADOW );
}

void D::DrawString( HFont font, int x, int y, Color color, uintptr_t alignment, const char* msg, ... )
{
	va_list va_alist;
	char buf[ 1024 ];
	va_start( va_alist, msg );
	_vsnprintf( buf, sizeof( buf ), msg, va_alist );
	va_end( va_alist );
	wchar_t wbuf[ 1024 ];
	MultiByteToWideChar( CP_UTF8, 0, buf, 256, wbuf, 256 );

	int r = 255, g = 255, b = 255, a = 255;
	color.GetColor( r, g, b, a );

	int width, height;
	g_pSurface->GetTextSize( font, wbuf, width, height );

	if ( alignment & FONT_RIGHT )
		x -= width;
	if ( alignment & FONT_CENTER )
		x -= width / 2;

	g_pSurface->DrawSetTextFont( font );
	g_pSurface->DrawSetTextColor( r, g, b, a );
	g_pSurface->DrawSetTextPos( x, y - height / 2 );
	g_pSurface->DrawPrintText( wbuf, wcslen( wbuf ) );
}

void D::DrawStringUnicode( HFont font, int x, int y, Color color, bool bCenter, const wchar_t* msg, ... )
{
	int r = 255, g = 255, b = 255, a = 255;
	color.GetColor( r, g, b, a );

	int iWidth, iHeight;

	g_pSurface->GetTextSize( font, msg, iWidth, iHeight );
	g_pSurface->DrawSetTextFont( font );
	g_pSurface->DrawSetTextColor( r, g, b, a );
	g_pSurface->DrawSetTextPos( !bCenter ? x : x - iWidth / 2, y - iHeight / 2 );
	g_pSurface->DrawPrintText( msg, wcslen( msg ) );
}

void D::DrawRect( int x, int y, int w, int h, Color col )
{
	g_pSurface->DrawSetColor( col );
	g_pSurface->DrawFilledRect( x, y, x + w, y + h );
}

void D::DrawRectRainbow( int x, int y, int width, int height, float flSpeed, float &flRainbow )
{
	Color colColor( 0, 0, 0 );

	flRainbow += flSpeed;
	if ( flRainbow > 1.f ) flRainbow = 0.f;

	for ( int i = 0; i < width; i++ )
	{
		float hue = ( 1.f / ( float ) width ) * i;
		hue -= flRainbow;
		if ( hue < 0.f ) hue += 1.f;

		Color colRainbow = colColor.FromHSB( hue, 1.f, 1.f );
		D::DrawRect( x + i, y, 1, height, colRainbow );
	}
}

void D::DrawRectGradientHorizontal( int x, int y, int width, int height, Color color1, Color color2 )
{
	float flDifferenceR = ( float ) ( color2.r() - color1.r() ) / ( float ) width;
	float flDifferenceG = ( float ) ( color2.g() - color1.g() ) / ( float ) width;
	float flDifferenceB = ( float ) ( color2.b() - color1.b() ) / ( float ) width;

	for ( float i = 0.f; i < width; i++ )
	{
		Color colGradient = Color( color1.r() + ( flDifferenceR * i ), color1.g() + ( flDifferenceG * i ), color1.b() + ( flDifferenceB * i ), color1.a() );
		D::DrawRect( x + i, y, 1, height, colGradient );
	}
}

void D::DrawPixel( int x, int y, Color col )
{
	g_pSurface->DrawSetColor( col );
	g_pSurface->DrawFilledRect( x, y, x + 1, y + 1 );
}

void D::DrawOutlinedRect( int x, int y, int w, int h, Color col )
{
	g_pSurface->DrawSetColor( col );
	g_pSurface->DrawOutlinedRect( x, y, x + w, y + h );
}

void D::DrawOutlinedCircle( int x, int y, int r, Color col )
{
	g_pSurface->DrawSetColor( col );
	g_pSurface->DrawOutlinedCircle( x, y, r, 1 );
}

void D::DrawLine( int x0, int y0, int x1, int y1, Color col )
{
	g_pSurface->DrawSetColor( col );
	g_pSurface->DrawLine( x0, y0, x1, y1 );
}

void D::DrawCorner( int iX, int iY, int iWidth, int iHeight, bool bRight, bool bDown, Color colDraw )
{
	int iRealX = bRight ? iX - iWidth : iX;
	int iRealY = bDown ? iY - iHeight : iY;

	if ( bDown && bRight )
		iWidth = iWidth + 1;

	D::DrawRect( iRealX, iY, iWidth, 1, colDraw );
	D::DrawRect( iX, iRealY, 1, iHeight, colDraw );

	D::DrawRect( iRealX, bDown ? iY + 1 : iY - 1, !bDown && bRight ? iWidth + 1 : iWidth, 1, Color( 0, 0, 0, 255 ) );
	D::DrawRect( bRight ? iX + 1 : iX - 1, bDown ? iRealY : iRealY - 1, 1, bDown ? iHeight + 2 : iHeight + 1, Color( 0, 0, 0, 255 ) );
}

void D::DrawPolygon( int count, Vertex_t* Vertexs, Color color )
{
	g_pSurface->DrawSetColor( color );
	g_pSurface->DrawTexturedPolygon( count, Vertexs );
}

void D::DrawRoundedBox( int x, int y, int w, int h, int r, int v, Color col )
{
	std::vector<Vertex_t> p;
	for ( int _i = 0; _i < 3; _i++ )
	{
		int _x = x + ( _i < 2 && r || w - r );
		int _y = y + ( _i % 3 > 0 && r || h - r );
		for ( int i = 0; i < v; i++ )
		{
			int a = RAD2DEG( ( i / v ) * -90 - _i * 90 );
			p.push_back( Vertex_t( Vector2D( _x + sin( a ) * r, _y + cos( a ) * r ) ) );
		}
	}

	D::DrawPolygon( 4 * ( v + 1 ), &p[ 0 ], col );
}

bool D::ScreenTransform( const Vector &point, Vector &screen )
{
	float w;
	const VMatrix &worldToScreen = g_pEngine->WorldToScreenMatrix();

	screen.x = worldToScreen[ 0 ][ 0 ] * point[ 0 ] + worldToScreen[ 0 ][ 1 ] * point[ 1 ] + worldToScreen[ 0 ][ 2 ] * point[ 2 ] + worldToScreen[ 0 ][ 3 ];
	screen.y = worldToScreen[ 1 ][ 0 ] * point[ 0 ] + worldToScreen[ 1 ][ 1 ] * point[ 1 ] + worldToScreen[ 1 ][ 2 ] * point[ 2 ] + worldToScreen[ 1 ][ 3 ];
	w = worldToScreen[ 3 ][ 0 ] * point[ 0 ] + worldToScreen[ 3 ][ 1 ] * point[ 1 ] + worldToScreen[ 3 ][ 2 ] * point[ 2 ] + worldToScreen[ 3 ][ 3 ];
	screen.z = 0.0f;

	bool behind = false;

	if ( w < 0.001f )
	{
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

bool D::WorldToScreen( const Vector &origin, Vector &screen )
{
	if ( !ScreenTransform( origin, screen ) )
	{
		int ScreenWidth, ScreenHeight;
		g_pEngine->GetScreenSize( ScreenWidth, ScreenHeight );
		float x = ScreenWidth / 2;
		float y = ScreenHeight / 2;
		x += 0.5 * screen.x * ScreenWidth + 0.5;
		y -= 0.5 * screen.y * ScreenHeight + 0.5;
		screen.x = x;
		screen.y = y;
		return true;
	}

	return false;
}

int D::GetStringWidth( HFont font, const char* msg, ... )
{
	va_list va_alist;
	char buf[ 1024 ];
	va_start( va_alist, msg );
	_vsnprintf( buf, sizeof( buf ), msg, va_alist );
	va_end( va_alist );
	wchar_t wbuf[ 1024 ];
	MultiByteToWideChar( CP_UTF8, 0, buf, 256, wbuf, 256 );

	int iWidth, iHeight;

	g_pSurface->GetTextSize( font, wbuf, iWidth, iHeight );

	return iWidth;
}

void D::Draw3DBox( Vector* boxVectors, Color color )
{
	Vector vec0, vec1, vec2, vec3,
		vec4, vec5, vec6, vec7;

	if ( D::WorldToScreen( boxVectors[ 0 ], vec0 ) &&
		 D::WorldToScreen( boxVectors[ 1 ], vec1 ) &&
		 D::WorldToScreen( boxVectors[ 2 ], vec2 ) &&
		 D::WorldToScreen( boxVectors[ 3 ], vec3 ) &&
		 D::WorldToScreen( boxVectors[ 4 ], vec4 ) &&
		 D::WorldToScreen( boxVectors[ 5 ], vec5 ) &&
		 D::WorldToScreen( boxVectors[ 6 ], vec6 ) &&
		 D::WorldToScreen( boxVectors[ 7 ], vec7 ) )
	{
		/*
		.+--5---+
		.8 4    6'|
		+--7---+'  11
		9   |  10  |
		|  ,+--|3--+
		|.0    | 2'
		+--1---+'
		*/

		Vector2D lines[ 12 ][ 2 ];
		//bottom of box
		lines[ 0 ][ 0 ] = { vec0.x, vec0.y };
		lines[ 0 ][ 1 ] = { vec1.x, vec1.y };
		lines[ 1 ][ 0 ] = { vec1.x, vec1.y };
		lines[ 1 ][ 1 ] = { vec2.x, vec2.y };
		lines[ 2 ][ 0 ] = { vec2.x, vec2.y };
		lines[ 2 ][ 1 ] = { vec3.x, vec3.y };
		lines[ 3 ][ 0 ] = { vec3.x, vec3.y };
		lines[ 3 ][ 1 ] = { vec0.x, vec0.y };

		lines[ 4 ][ 0 ] = { vec0.x, vec0.y };
		lines[ 4 ][ 1 ] = { vec6.x, vec6.y };

		// top of box
		lines[ 5 ][ 0 ] = { vec6.x, vec6.y };
		lines[ 5 ][ 1 ] = { vec5.x, vec5.y };
		lines[ 6 ][ 0 ] = { vec5.x, vec5.y };
		lines[ 6 ][ 1 ] = { vec4.x, vec4.y };
		lines[ 7 ][ 0 ] = { vec4.x, vec4.y };
		lines[ 7 ][ 1 ] = { vec7.x, vec7.y };
		lines[ 8 ][ 0 ] = { vec7.x, vec7.y };
		lines[ 8 ][ 1 ] = { vec6.x, vec6.y };


		lines[ 9 ][ 0 ] = { vec5.x, vec5.y };
		lines[ 9 ][ +1 ] = { vec1.x, vec1.y };

		lines[ 10 ][ 0 ] = { vec4.x, vec4.y };
		lines[ 10 ][ 1 ] = { vec2.x, vec2.y };

		lines[ 11 ][ 0 ] = { vec7.x, vec7.y };
		lines[ 11 ][ 1 ] = { vec3.x, vec3.y };

		for ( int i = 0; i < 12; i++ )
		{
			D::DrawLine( lines[ i ][ 0 ].x, lines[ i ][ 0 ].y, lines[ i ][ 1 ].x, lines[ i ][ 1 ].y, color );
		}
	}
}

void D::DrawCircle( float x, float y, float r, float s, Color color )
{
	float Step = M_PI * 2.0 / s;
	for ( float a = 0; a < ( M_PI*2.0 ); a += Step )
	{
		float x1 = r * cos( a ) + x;
		float y1 = r * sin( a ) + y;
		float x2 = r * cos( a + Step ) + x;
		float y2 = r * sin( a + Step ) + y;

		DrawLine( x1, y1, x2, y2, color );
	}
}

void D::CornerBox( const int32_t x, const int32_t y, const int32_t w, const int32_t h, const bool outlined, const Color& color, const Color& outlined_color )
{
	if ( !Vars.Visuals.PlayerEsp.bBox )
		return;

	auto corner = [ & ] ( const int32_t _x, const int32_t _y, const int32_t width, const int32_t height, const bool right_side, const bool down, const Color& _color, const bool _outlined = false, const Color& _outlined_color = 0xFF000000 ) -> void
	{
		const auto corner_x = right_side ? _x - width : _x;
		const auto corner_y = down ? _y - height : _y;
		const auto corner_w = down && right_side ? width + 1 : width;

		D::DrawRect( corner_x, _y, corner_w, 1, _color );
		D::DrawRect( _x, corner_y, 1, height, _color );

		if ( _outlined )
		{
			D::DrawRect( corner_x, down ? _y + 1 : _y - 1, !down && right_side ? corner_w + 1 : corner_w, 1, _outlined_color );
			D::DrawRect( right_side ? _x + 1 : _x - 1, down ? corner_y : corner_y - 1, 1, down ? height + 2 : height + 1, _outlined_color );
		}
	};

	corner( x - w, y, w / 2, w / 2, false, false, color, outlined, outlined_color );
	corner( x - w, y + h, w / 2, w / 2, false, true, color, outlined, outlined_color );
	corner( x + w, y, w / 2, w / 2, true, false, color, outlined, outlined_color );
	corner( x + w, y + h, w / 2, w / 2, true, true, color, outlined, outlined_color );
}

void D::FilledCornerBox( const int32_t x, const int32_t y, const int32_t w, const int32_t h, const bool outlined, const Color& color )
{
	if ( !Vars.Visuals.PlayerEsp.bFilled )
		return;

	if ( outlined )
		D::DrawOutlinedRect( x - w, y, w * 2, h, color );
	else
		D::DrawRect( x - w, y, w * 2, h, color );
}

void D::DrawTexturedPoly( int n, Vertex_t* vertice, Color col )
{
	static int texture_id = g_pSurface->CreateNewTextureID( true );
	static unsigned char buf[ 4 ] = { 255, 255, 255, 255 };
	g_pSurface->DrawSetTextureRGBA( texture_id, buf, 1, 1 );
	g_pSurface->DrawSetColor( col );
	g_pSurface->DrawSetTexture( texture_id );
	g_pSurface->DrawTexturedPolygon( n, vertice );
}

void D::DrawTexturedPoly( int n, VertexCol_t* vertice )
{
	static int texture_id = g_pSurface->CreateNewTextureID( true );
	static unsigned char buf[ 4 ] = { 255, 255, 255, 255 };
	g_pSurface->DrawSetTextureRGBA( texture_id, buf, 1, 1 );
	g_pSurface->DrawSetColor( vertice->m_Col );
	g_pSurface->DrawSetTexture( texture_id );
	g_pSurface->DrawTexturedPolygon( n, &vertice->m_Vertex );
}

void D::DrawFilledCircle( Vector2D center, Color color, float radius, float points )
{
	std::vector<Vertex_t> vertices;
	float step = ( float ) M_PI * 2.0f / points;

	for ( float a = 0; a < ( M_PI * 2.0f ); a += step )
		vertices.push_back( Vertex_t( Vector2D( radius * cosf( a ) + center.x, radius * sinf( a ) + center.y ) ) );

	DrawTexturedPoly( ( int ) points, vertices.data(), color );
}

void D::DrawFilled3DBox( Vector origin, int width, int height, Color outline, Color filling )
{
	float difw = float( width / 2 );
	float difh = float( height / 2 );
	Vector boxVectors[ 8 ] =
	{
		Vector( origin.x - difw, origin.y - difh, origin.z - difw ),
		Vector( origin.x - difw, origin.y - difh, origin.z + difw ),
		Vector( origin.x + difw, origin.y - difh, origin.z + difw ),
		Vector( origin.x + difw, origin.y - difh, origin.z - difw ),
		Vector( origin.x - difw, origin.y + difh, origin.z - difw ),
		Vector( origin.x - difw, origin.y + difh, origin.z + difw ),
		Vector( origin.x + difw, origin.y + difh, origin.z + difw ),
		Vector( origin.x + difw, origin.y + difh, origin.z - difw ),
	};

	static Vector vec0, vec1, vec2, vec3,
		vec4, vec5, vec6, vec7;

	if ( D::WorldToScreen( boxVectors[ 0 ], vec0 ) &&
		 D::WorldToScreen( boxVectors[ 1 ], vec1 ) &&
		 D::WorldToScreen( boxVectors[ 2 ], vec2 ) &&
		 D::WorldToScreen( boxVectors[ 3 ], vec3 ) &&
		 D::WorldToScreen( boxVectors[ 4 ], vec4 ) &&
		 D::WorldToScreen( boxVectors[ 5 ], vec5 ) &&
		 D::WorldToScreen( boxVectors[ 6 ], vec6 ) &&
		 D::WorldToScreen( boxVectors[ 7 ], vec7 ) )
	{
		Vector2D lines[ 12 ][ 2 ];
		lines[ 0 ][ 0 ] = vec0;
		lines[ 0 ][ 1 ] = vec1;
		lines[ 1 ][ 0 ] = vec1;
		lines[ 1 ][ 1 ] = vec2;
		lines[ 2 ][ 0 ] = vec2;
		lines[ 2 ][ 1 ] = vec3;
		lines[ 3 ][ 0 ] = vec3;
		lines[ 3 ][ 1 ] = vec0;

		// top of box
		lines[ 4 ][ 0 ] = vec4;
		lines[ 4 ][ 1 ] = vec5;
		lines[ 5 ][ 0 ] = vec5;
		lines[ 5 ][ 1 ] = vec6;
		lines[ 6 ][ 0 ] = vec6;
		lines[ 6 ][ 1 ] = vec7;
		lines[ 7 ][ 0 ] = vec7;
		lines[ 7 ][ 1 ] = vec4;

		lines[ 8 ][ 0 ] = vec0;
		lines[ 8 ][ 1 ] = vec4;

		lines[ 9 ][ 0 ] = vec1;
		lines[ 9 ][ 1 ] = vec5;

		lines[ 10 ][ 0 ] = vec2;
		lines[ 10 ][ 1 ] = vec6;

		lines[ 11 ][ 0 ] = vec3;
		lines[ 11 ][ 1 ] = vec7;

		auto DrawPolys = [ & ] ( const std::vector<std::vector<Vector2D>>& vecs ) -> void
		{
			g_pSurface->DrawSetColor( filling );
			for ( auto it : vecs )
			{
				std::vector<Vertex_t> vertices = { it[ 0 ],it[ 1 ],it[ 2 ],it[ 3 ] };
				g_pSurface->DrawTexturedPolygon( 4, vertices.data() );
				vertices.clear();
				vertices = { it[ 3 ],it[ 2 ],it[ 1 ],it[ 0 ] };
				g_pSurface->DrawTexturedPolygon( 4, vertices.data() );
			}
		};

		std::vector<Vector2D> front = { vec0,vec1,vec2,vec3 };
		std::vector<Vector2D> back = { vec4,vec5,vec6,vec7 };
		std::vector<Vector2D> left = { vec3,vec2,vec7,vec6 };
		std::vector<Vector2D> right = { vec0,vec1,vec5,vec4 };
		std::vector<Vector2D> top = { vec2,vec1,vec5,vec6 };
		std::vector<Vector2D> bottom = { vec0,vec4,vec7,vec3 };

		std::vector<std::vector<Vector2D>> vectors{ front, back,left,right,top,bottom };

		DrawPolys( vectors );

		for ( int i = 0; i < 12; i++ )
			D::DrawLine( lines[ i ][ 0 ].x, lines[ i ][ 0 ].y, lines[ i ][ 1 ].x, lines[ i ][ 1 ].y, outline );
	}
}