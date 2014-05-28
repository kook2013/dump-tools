/*	This file is part of dump-tools.

	dump-tools is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	dump-tools is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with dump-tools.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DUMP_PLANE_HPP
#define DUMP_PLANE_HPP

#include <QFile>

#include <iostream>
#include <stdint.h>
#include <cstring>
#include <vector>


struct Plane{
	private:
		uint32_t width{ 0 };
		uint32_t height{ 0 };
		uint16_t depth{ 0 };
		uint16_t config{ 0 };
		
		std::vector<uint8_t> data;
		
	public:
		uint32_t getWidth() const{ return width; }
		uint32_t getHeight() const{ return height; }
		uint8_t getDepth() const{ return depth; }
		uint16_t getConfig() const{ return config; }
		
		int byteCount() const{ return (depth-1) / 8 + 1; }

		const uint8_t* constScanline( unsigned y ) const {
			return (const uint8_t*)data.data() + (uint64_t)width * y * byteCount();
		}
		
	public:
		bool read( QIODevice &dev );
		bool write( QIODevice &dev );
		int32_t size() const{ return width*height*((depth + 7) / 8); }
		
		uint16_t read_16( QIODevice &dev ){
			char byte1, byte2;
			if( !dev.getChar( &byte1 ) )
				return 0;
			if( !dev.getChar( &byte2 ) )
				return 0;
			return ((uint16_t)byte2 << 8) + (uint8_t)byte1;
		}
		
		uint32_t read_32( QIODevice &dev ){
			uint16_t byte1 = read_16( dev );
			uint32_t byte2 = read_16( dev );
			return (byte2 << 16) + byte1;
		}
		
		void write_16( QIODevice &dev, uint16_t val ){
			dev.write( (char*)&val, 2 );
		}
		void write_32( QIODevice &dev, uint32_t val ){
			dev.write( (char*)&val, 4 );
		}
		
		void compression_ratio( unsigned compressed_size ) const{
			std::cout << "\tCompressed (" << width << "x" << height << ") to " << 100.0-compressed_size*100.0/size() << "%\n";
		}
};

#endif
