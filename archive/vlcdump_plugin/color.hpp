/*
	This file is part of Overmix.

	Overmix is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Overmix is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Overmix.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COLOR_H
#define COLOR_H

#include <QCoreApplication>
#include <QColor>
#include <cmath>

typedef unsigned short color_type;

struct color{
	color_type r;
	color_type b;
	color_type g;
	color_type a;
	
	private:
		static color_type sRgb2linear( color_type value ){
			double v = value / (double)(255*256);
			v = ( v <= 0.04045 ) ? v / 12.92 : std::pow( (v+0.055)/1.055, 2.4 );
			return (color_type)( v *255*256 +0.5 );
		}
		static color_type linear2sRgb( color_type value ){
			double v = value / (double)(255*256);
			v = ( v <= 0.0031308 ) ? 12.92 * v : 1.055*std::pow( v, 1.0/2.4 ) - 0.055;
			return (color_type)( v *255*256 +0.5 );
		}
	
	
	public:
		void linearize(){
			r = sRgb2linear( r );
			g = sRgb2linear( g );
			b = sRgb2linear( b );
			a = sRgb2linear( a );
		}
		void sRgb(){
			r = linear2sRgb( r );
			g = linear2sRgb( g );
			b = linear2sRgb( b );
			a = linear2sRgb( a );
		}
		
		static void test(){
			for( int i=0; i<=255*256; i++ ){
				color_type i2 = linear2sRgb( sRgb2linear( i ) );
				if( i != i2 )
					qDebug( "didn't match: %d %d", i, i2 );
			}
		}
		
		
		color rec709_to_rgb(){
			//*
			double y = r / (255*256.0);
			double u = g / (255*256.0);
			double v = b / (255*256.0);
			
			//Remove foot- and head-room
			y = (y - (16 / 255.0)) * ( 1 + 16.0 / 255.0 + (256-235) / 255.0 );
			u = (u - (16 / 255.0)) * ( 1 + 16.0 / 255.0 + (256-240) / 255.0 );
			v = (v - (16 / 255.0)) * ( 1 + 16.0 / 255.0 + (256-240) / 255.0 );
			
			//Don't let it outside the allowed range
			y = (y < 0 ) ? 0 : y;
			u = (u < 0 ) ? 0 : u;
			v = (v < 0 ) ? 0 : v;
			y = (y > 1 ) ? 1 : y;
			u = (u > 1 ) ? 1 : u;
			v = (v > 1 ) ? 1 : v;
			
			//Move chroma
			u -= 0.5;
			v -= 0.5;
			
			double rr = y + 1.5701 * v;
			double rg = y - 0.1870 * u - 0.4664 * v;
			double rb = y + 1.8556 * u;
			
			//Don't let it outside the allowed range
			rr = (rr < 0 ) ? 0 : rr;
			rg = (rg < 0 ) ? 0 : rg;
			rb = (rb < 0 ) ? 0 : rb;
			rr = (rr > 1 ) ? 1 : rr;
			rg = (rg > 1 ) ? 1 : rg;
			rb = (rb > 1 ) ? 1 : rb;
			
			rr = std::pow( std::pow( rr, 2.4 ), 1/2.2 );
			rg = std::pow( std::pow( rg, 2.4 ), 1/2.2 );
			rb = std::pow( std::pow( rb, 2.4 ), 1/2.2 );
			
			//Transform range
			rr = (rr) * 255*256;
			rg = (rg) * 255*256;
			rb = (rb) * 255*256;
			
			return color( rr, rg, rb, a );
		}
		
	void clear(){
		r = b = g = a = 0;
	}
	
	void trunc( color_type max ){
		r = ( r > max ) ? max : r;
		g = ( g > max ) ? max : g;
		b = ( b > max ) ? max : b;
		a = ( a > max ) ? max : a;
	}
	
	color difference( color c ){
		c.diff( *this );
		return c;
	}
	void diff( color c ){
		r = ( c.r > r ) ? c.r - r : r - c.r;
		g = ( c.g > g ) ? c.g - g : g - c.g;
		b = ( c.b > b ) ? c.b - b : b - c.b;
		a = ( c.a > a ) ? c.a - a : a - c.a;
	}
	
	color_type gray(){
		//This function corresponds to qGray()
		return ( r*11 + g*16 + b*5 ) / 32;
	}
	
	color(){
		clear();
	}
	color( color_type r, color_type g, color_type b, color_type a = 255*256 ){
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	color( color* c ){
		r = c->r;
		g = c->g;
		b = c->b;
		a = c->a;
	}
	color( QRgb c ){
		r = qRed( c ) * 256;
		g = qGreen( c ) * 256;
		b = qBlue( c ) * 256;
		a = qAlpha( c ) * 256;
	//	linearize();
	}
	
	color& operator+=( const color &rhs ){
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;
		return *this;
	}
	
	color& operator-=( const color &rhs ){
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		a -= rhs.a;
		return *this;
	}
	
	color& operator+=( const int &rhs ){
		r += rhs;
		g += rhs;
		b += rhs;
		a += rhs;
		return *this;
	}
	
	color& operator*=( const int &rhs ){
		r *= rhs;
		g *= rhs;
		b *= rhs;
		a *= rhs;
		return *this;
	}
	
	color& operator/=( const int &rhs ){
		r /= rhs;
		g /= rhs;
		b /= rhs;
		a /= rhs;
		return *this;
	}
	
	color& operator<<=( const int &rhs ){
		r <<= rhs;
		g <<= rhs;
		b <<= rhs;
		a <<= rhs;
		return *this;
	}
	
	color& operator>>=( const int &rhs ){
		r >>= rhs;
		g >>= rhs;
		b >>= rhs;
		a >>= rhs;
		return *this;
	}
	
	const color operator+( const color &other ) const{
		return color(*this) += other;
	}
	const color operator+( const int &other ) const{
		return color(*this) += other;
	}
	const color operator-( const color &other ) const{
		return color(*this) -= other;
	}
	const color operator*( const int &other ) const{
		return color(*this) *= other;
	}
	const color operator/( const int &other ) const{
		return color(*this) /= other;
	}
	const color operator<<( const int &other ) const{
		return color(*this) <<= other;
	}
	const color operator>>( const int &other ) const{
		return color(*this) >>= other;
	}
};


class ColorAvg{
	private:
		unsigned r;
		unsigned g;
		unsigned b;
		unsigned a;
		unsigned amount;
		
	public:
		ColorAvg(){
			r = g = b = a = 0;
			amount = 0;
		}
		
		unsigned size() const{ return amount; }
		
		ColorAvg& operator+=( const color &rhs ){
			r += rhs.r;
			g += rhs.g;
			b += rhs.b;
			a += rhs.a;
			++amount;
			return *this;
		}
		
		ColorAvg& operator-=( const color &rhs ){
			r -= rhs.r;
			g -= rhs.g;
			b -= rhs.b;
			a -= rhs.a;
			--amount;
			return *this;
		}
		
		color get_color() const{
			if( amount )
				return color(
						r / amount
					,	g / amount
					,	b / amount
					,	a / amount
					);
			else
				return color();
		}
		
		operator color(){ return get_color(); }
};

#endif