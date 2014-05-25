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

#ifndef DUMP_HPP
#define DUMP_HPP


#include <QImage>
#include <cstdint>

class Dump{
	private:
		QIODevice* dev;
		
		static std::uint16_t* gamma;
		
		static void generate_gamma();
		
	public:
		Dump( QIODevice* dev ) : dev( dev ) { };
		
		QImage to_qimage() const;
};



#endif