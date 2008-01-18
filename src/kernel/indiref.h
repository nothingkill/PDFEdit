/*                                                                              
 * PDFedit - free program for PDF document manipulation.                        
 * Copyright (C) 2006, 2007  PDFedit team:      Michal Hocko, 
 *                                              Miroslav Jahoda,       
 *                                              Jozef Misutka, 
 *                                              Martin Petricek                                             
 *
 * Project is hosted on http://sourceforge.net/projects/pdfedit                                                                      
 */ 
// vim:tabstop=4:shiftwidth=4:noexpandtab:textwidth=80
#ifndef _INDIREF_H_
#define _INDIREF_H_

//=====================================================================================
namespace pdfobjects {
//=====================================================================================


/** 
 * Two numbers specifying indirect reference of a pdf object. 
 */
typedef 
struct IndiRef
{
	/** Object identification number. */
	typedef unsigned int ObjNum;
	/** Object generation number. */
	typedef unsigned int GenNum;

	ObjNum	num; /**< Objects pdf identification number. */
	GenNum	gen; /**< Objects pdf generation number. */

	static const ObjNum invalidnum = 0;	/**< Invalid object identification number. */
	static const GenNum invalidgen = 0; /**< Invalid object generation number. */

	/** 
	 * Empty constructor.
	 * Initialize identification and generation number to invalid reference.
	 */
	IndiRef() : num(invalidnum), gen(invalidgen) {}
	
	/** 
	 * Constructor.
	 * Stores supplied identification and generation numbers.
	 * 
	 * @param ref Indirect Reference.
	 */
	IndiRef(const IndiRef& ref) : num(ref.num), gen(ref.gen)	{}

	/** 
	 * Constructor.
	 * Stores supplied identification and generation numbers.
	 * 
	 * @param ref Xpdf reference.
	 */
	IndiRef(const ::Ref& ref) : num(ref.num), gen(ref.gen) {}

	/** 
	 * Constructor.
	 * Stores supplied identification and generation numbers.
	 * 
	 * @param _num Object number.
	 * @param _gen Generation number.
	 */
	IndiRef(int _num, int _gen) : num(_num), gen(_gen) {}

	/** Copy constructor. */
	IndiRef& operator= (const IndiRef& _r) { num = _r.num; gen = _r.gen; return *this;}
	/** Equality operator. */
	bool operator== (const IndiRef& _r) const { return (num == _r.num && gen == _r.gen) ? true : false;}
			
} IndiRef;

//=====================================================================================
} // pdfobjects 
//=====================================================================================


#endif  //_INDIREF_H_
