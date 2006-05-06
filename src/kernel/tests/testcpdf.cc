// vim:tabstop=4:shiftwidth=4:noexpandtab:textwidth=80

/*
 * $RCSfile$
 *
 * $Log$
 * Revision 1.14  2006/05/06 08:56:57  hockm0bm
 * tests improved - all of them are successfull - everything tested seems to work
 *
 * Revision 1.13  2006/04/28 17:18:17  hockm0bm
 * * instancingTC
 *         - just skeleton
 * * revisionsTC removed from pdfs loop in Test method
 * * indirectPropertyTC
 *         - first test cases
 *
 * Revision 1.12  2006/04/27 18:35:50  hockm0bm
 * revisionsTC new test cases
 *        - changeRevision seams to be tested quite well
 *
 * Revision 1.11  2006/04/25 02:28:05  misuj1am
 *
 * -- improved cpage, cstream, tests
 *
 * Revision 1.10  2006/04/23 23:20:40  misuj1am
 *
 * -- improved: zero page (incorrect) document handling
 *
 * Revision 1.9  2006/04/23 22:09:48  hockm0bm
 * * pageIterationTC finished
 *         - all tests are ok
 * * pageManipulationTC
 *         - almost finised (interNode removing is TODO)
 *         - all tests until now are ok
 * * revisionsTC
 *         - just skeleton
 *
 * Revision 1.8  2006/04/23 11:16:48  hockm0bm
 * pageManipulationTC added
 *         - insertPage, removePage test cases
 *
 * Revision 1.7  2006/04/22 19:46:12  hockm0bm
 * test case Test added to CPPUnit tests of CPdf suite
 *
 * Revision 1.6  2006/04/22 19:32:20  hockm0bm
 * * old test style replaced by CPPUINT TestCPdf class
 * * TestCPdf::pageIterationTC method implemented
 *
 *
 *
 */

#include "testmain.h"
#include "testcpdf.h"
#include "../factories.h"
#include "../cobjecthelpers.h"
#include "../cpdf.h"

class TestCPdf: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestCPdf);
		CPPUNIT_TEST(Test);
	CPPUNIT_TEST_SUITE_END();

	typedef std::vector<CPdf *> PdfList;
	PdfList	pdfs;
public:

	virtual ~TestCPdf()
	{
	}

	void pageIterationTC(CPdf * pdf)
	{
	using namespace boost;
	using namespace utils;

		printf("%s\n", __FUNCTION__);
		
		// getPage and getPagePosition must match for all pages
		printf("TC01:\tPageCount, getPage, getPagePosition\n");
		size_t pageCount=pdf->getPageCount();
		if (0 == pageCount)
			return;
		for(size_t i=1; i<=pageCount; i++)
		{
			shared_ptr<CPage> page=pdf->getPage(i);
			size_t pos=pdf->getPagePosition(page);
			CPPUNIT_ASSERT(i==pos);
		}

		// getPage, getNextPage, getPrevPage must match for all pages
		printf("TC02:\tgetPage, getNextPage, getPrevPage\n");
		for(size_t i=1; i<=pageCount; i++)
		{
			shared_ptr<CPage> pos=pdf->getPage(i);
			if(i>1)
			{
				shared_ptr<CPage> prev=pdf->getPage(i-1);
				CPPUNIT_ASSERT(pdf->getPrevPage(pos)==prev);
			}
			if(i<pageCount)
			{
				shared_ptr<CPage> next=pdf->getPage(i+1);
				CPPUNIT_ASSERT(pdf->getNextPage(pos)==next);
			}
		}
		
		printf("TC03:\tgetPage, getFirstPage and getLastPage test\n");
		if(pdf->getPageCount())
		{
			CPPUNIT_ASSERT(pdf->getPage(1)==pdf->getFirstPage());
			CPPUNIT_ASSERT(pdf->getPage(pdf->getPageCount())==pdf->getLastPage());
		}

		// out of range page positions must throw
		printf("TC04:\tgetPage, getNextPage, getPrevPage out of range test\n");

		// 0 page is out of range
		try
		{
			pdf->getPage(0);
			CPPUNIT_FAIL("getPage(0) should have failed");
		}catch(PageNotFoundException &e)
		{
			// ok, exception has been thrown
		}
		
		// pageCount + 1 is out of range
		try
		{
			pdf->getPage(pdf->getPageCount()+1);
			CPPUNIT_FAIL("getPage(%d) should have filed");
		}catch(PageNotFoundException &e)
		{
			// ok, exception has been thrown
		}
		// getPrevPage(getFirstPage()) is out of range
		try
		{
			pdf->getPrevPage(pdf->getFirstPage());
			CPPUNIT_FAIL("getPrevPage(getFirstPage()) should have failed");
		}catch(PageNotFoundException &e)
		{
			// ok, exception has been thrown
		}
		
		// getNextPage(getLastPage()) is out of range
		try
		{
			pdf->getNextPage(pdf->getLastPage());
			CPPUNIT_FAIL("getNextPage(getLastPage()) should have failed");
		}catch(PageNotFoundException &e)
		{
			// ok, exception has been thrown
		}

		printf("TC05:\thasNextPage, hasPrevPage test\n");

		if(pdf->getPageCount())
		{
			// first page hasPrevPage should return false
			CPPUNIT_ASSERT(!pdf->hasPrevPage(pdf->getFirstPage()));
			// last page hasNextPage should return false
			CPPUNIT_ASSERT(!pdf->hasNextPage(pdf->getLastPage()));
			// hasNextPage and hasPrevPage should return true for all other 
			// pages
			for(size_t i=2; i<pageCount; i++)
			{
				CPPUNIT_ASSERT(pdf->hasPrevPage(pdf->getPage(i)));
				CPPUNIT_ASSERT(pdf->hasNextPage(pdf->getPage(i)));
			}
		}

		printf("TC06:\tgetPagePosition, getNextPage, getPrevPage with fake page parameter test\n");
		// page from empty page dictionary - no pdf specified inside
		shared_ptr<CDict> fakeDict1(CDictFactory::getInstance());
		shared_ptr<CPage> fake1(new CPage(fakeDict1));
		Object fakeXpdfDict;
		XRef * fakeXref=pdf->getCXref();
		fakeXpdfDict.initDict(fakeXref);
		IndiRef fakeIndiRef={10, 0};
		shared_ptr<CDict> fakeDict2(CDictFactory::getInstance(*pdf, fakeIndiRef, fakeXpdfDict));
		shared_ptr<CPage> fake2(new CPage(fakeDict2));

		// getPagePosition should fail on both fakes
		try
		{
			pdf->getPagePosition(fake1);
			CPPUNIT_FAIL("getPagePosition(fake1) should have failed");
		}catch(PageNotFoundException & e)
		{
			// ok, exception has been thrown
		}
		try
		{
			pdf->getPagePosition(fake2);
			CPPUNIT_FAIL("getPagePosition(fake2) should have failed");
		}catch(PageNotFoundException & e)
		{
			// ok, exception has been thrown
		}

		// getPrevPage should fail on both fakes
		try
		{
			pdf->getPrevPage(fake1);
			CPPUNIT_FAIL("getPrevPage(fake1) should have failed");
		}catch(PageNotFoundException & e)
		{
			// ok, exception has been thrown
		}
		try
		{
			pdf->getPrevPage(fake2);
			CPPUNIT_FAIL("getPrevPage(fake2) should have failed");
		}catch(PageNotFoundException & e)
		{
			// ok, exception has been thrown
		}

		// getNextPage should fail on both fakes
		try
		{
			pdf->getNextPage(fake1);
			CPPUNIT_FAIL("getNextPage(fake1) should have failed");
		}catch(PageNotFoundException & e)
		{
			// ok, exception has been thrown
		}
		try
		{
			pdf->getNextPage(fake2);
			CPPUNIT_FAIL("getNextPage(fake2) should have failed");
		}catch(PageNotFoundException & e)
		{
			// ok, exception has been thrown
		}
	}

	void pageManipulationTC(CPdf * pdf)
	{
	using namespace boost;
	using namespace utils;

		printf("%s\n", __FUNCTION__);

		printf("TC01:\tremovePage, insertPage changes pageCount\n");
		size_t pageCount=pdf->getPageCount();
		if (0 == pageCount)
			return;
		shared_ptr<CPage> page=pdf->getPage(1);
		// remove page implies pageCount decrementation test
		pdf->removePage(1);
		CPPUNIT_ASSERT(pageCount-1==pdf->getPageCount());

		// insert page implies pageCount incrementation
		// and newPage must be different instance than original one
		shared_ptr<CPage> newPage=pdf->insertPage(page, 1);
		CPPUNIT_ASSERT(pageCount==pdf->getPageCount());

		printf("TC02:\tinsertPage returns different instance than parameter\n");
		CPPUNIT_ASSERT(page!=newPage);
		CPPUNIT_ASSERT(page->getDictionary()!=newPage->getDictionary());

		// page count is same as in original file now

		printf("TC03:\tremovePage out of range test\n");
		// remove from 0 page should fail
		try
		{
			pdf->removePage(0);
			CPPUNIT_FAIL("removePage should have failed");
		}catch(PageNotFoundException & e)
		{
			// everything ok
		}
		// remove from pageCount()+1 should fail
		try
		{
			pdf->removePage(pdf->getPageCount()+1);
			CPPUNIT_FAIL("removePage should have failed");
		}catch(PageNotFoundException & e)
		{
			// everything ok
		}

		printf("TC04:\tinsert page never causes ambiguous page tree\n");
		// inserts first page again and tries to to call getNodePosition
		shared_ptr<CPage> addedPage=pdf->insertPage(pdf->getFirstPage(), 1);
		size_t pos=getNodePosition(*pdf, addedPage->getDictionary());
		CPPUNIT_ASSERT(pos==1);
		// removes added page
		pdf->removePage(pos);

		printf("TC05:\tremoved page is no longer available test\n");
		// try to remove last page
		shared_ptr<CPage> removedPage=pdf->getLastPage();
		IndiRef removedPageRef=removedPage->getDictionary()->getIndiRef();
		pos=pdf->getPagePosition(removedPage);
		CPPUNIT_ASSERT(pos==pdf->getPageCount());
		pdf->removePage(pdf->getPageCount());
		// FIXME uncoment when ready
		//CPPUNIT_ASSERT(! removePage->isValid());
		try
		{
			pos=pdf->getPagePosition(removedPage);
			CPPUNIT_FAIL("getPagePosition on removed page should have failed.");
		}catch(PageNotFoundException & e)
		{
			/* ok, it should fail */
		}
		// restore to previous state and returns back last page
		pdf->insertPage(removedPage, pos);
		
		printf("TC06:\tremoving page's dictionary invalidates page\n");
		pos=pdf->getPageCount()/2;
		// if we have just one page, use it
		if(!pos)
			pos=1;
		page=pdf->getPage(pos);
		shared_ptr<CDict> pageDict=page->getDictionary();
		shared_ptr<CRef> pageRef(CRefFactory::getInstance(pageDict->getIndiRef()));
		// gets parent of page dictionary and removes reference of this page
		// from Kids array
		shared_ptr<CRef> parentRef=IProperty::getSmartCObjectPtr<CRef>(pageDict->getProperty("Parent"));
		shared_ptr<CDict> parentDict=IProperty::getSmartCObjectPtr<CDict>(
				pdf->getIndirectProperty(getValueFromSimple<CRef, pRef, IndiRef>(parentRef)));
		shared_ptr<CArray> kidsArray=IProperty::getSmartCObjectPtr<CArray>(
				parentDict->getProperty("Kids"));
		vector<CArray::PropertyId> positions;
		getPropertyId<CArray, vector<CArray::PropertyId> >(kidsArray, pageRef, positions);
		CPPUNIT_ASSERT(positions.size()>0);
		// we knows that position is not ambiguous so use 1st in positions array
		kidsArray->delProperty(positions[0]);
		// FIXME uncoment when ready
		//CPPUNIT_ASSERT(! page->isValid());
		try
		{
			pdf->getPagePosition(page);
			CPPUNIT_FAIL("getPagePosition on removed page should have failed");
		}catch(PageNotFoundException & e)
		{
			/* ok, it should fail */
		}
		// inserts back removed page
		pdf->insertPage(page, pos);
		
		printf("TC07:\tremovig inter node removes all pages under\n");
		// uses 1st internode kid from page tree root dictionary
		shared_ptr<CRef> rootRef=IProperty::getSmartCObjectPtr<CRef>(pdf->getDictionary()->getProperty("Pages"));
		shared_ptr<CDict> rootDict=getDictFromRef(rootRef);
		shared_ptr<CArray> rootKids=IProperty::getSmartCObjectPtr<CArray>(rootDict->getProperty("Kids"));
		shared_ptr<CDict> interNode;
		size_t interPos;
		for(size_t i=0; i<rootKids->getPropertyCount(); i++)
		{
			shared_ptr<CRef> kidRef=IProperty::getSmartCObjectPtr<CRef>(rootKids->getProperty(i));
			shared_ptr<CDict> kidDict=getDictFromRef(kidRef);
			if(getNameFromDict("Type", kidDict)=="Pages")
			{
				interNode=kidDict;
				interPos=i;
				break;
			}
		}
		if(!interNode)
			printf("\t\tThis file is not suitable for this test\n");
		else
		{
			// collects all descendants from interNode
			// TODO test case for isDescendant
			vector<shared_ptr<CPage> > descendants;
			size_t pageCount=pdf->getPageCount();
			for(size_t i=1; i<pageCount; i++)
			{
				shared_ptr<CPage> page=pdf->getPage(i);	
				if(isDescendant(*pdf, interNode->getIndiRef(), page->getDictionary()))
					descendants.push_back(page);
			}

			// all descendants are collected, we can remove interNode reference
			// from interPos from rootKids
			rootKids->delProperty(interPos);

			// page count has to be decreased by descendants.size()
			CPPUNIT_ASSERT(pdf->getPageCount()+descendants.size()==pageCount);

			// all pages from descendants are not available
			for(vector<shared_ptr<CPage> >::iterator i=descendants.begin();i!=descendants.end(); i++)
			{
				try
				{
					pos=pdf->getPagePosition(*i);
					CPPUNIT_FAIL("getPagePosition on removed page should have failed");
				}catch(PageNotFoundException &e)
				{
					/* ok */
				}
			}
			// test passed
		}
	}

/** Multiversion file name. */
#define MV_F "tests/multiversion.pdf"

/** Multiversion revisin count. */
#define MV_RC  4

#define TRY_READONLY_OP(method, errorMsg)\
	{\
		try\
		{\
			method;\
			CPPUNIT_FAIL(errorMsg);\
		}catch(ReadOnlyDocumentException &e)\
		{\
			/* passed */\
		}\
	}
	void revisionsTC()
	{
	using namespace boost;

		printf("%s\n", __FUNCTION__);

		// opens special test file
		printf("Using file \"%s\"\n", MV_F);
		CPdf *pdf=getTestCPdf(MV_F);
		CPdf::OpenMode mode=pdf->getMode();

		// number of revision must match
		printf("TC01:\tRevisions count test\n");
		CPPUNIT_ASSERT(pdf->getRevisionsCount()==MV_RC);

		printf("TC02:\tchangeRevision, getActualRevision tests\n");
		for(CPdf::revision_t i=0; i<pdf->getRevisionsCount(); i++)
		{
			pdf->changeRevision(i);
			CPPUNIT_ASSERT(pdf->getActualRevision()==i);
		}

		printf("TC03:\tgetPageCount for revision test\n");
		// starts from the oldest one - each revision has number of pages one
		// more than revision number
		for(CPdf::revision_t i=pdf->getRevisionsCount()-1; ; i--)
		{
			pdf->changeRevision(i);
			CPPUNIT_ASSERT(pdf->getPageCount()==i+1);
			if(i==0)
				break;
			
		}

		printf("TC04:\tgetCXref::getNumObjects is same for all revisions\n");
		// no changes has been done, so all revisions has to have same number
		// of objects
		int number=0;
		for(CPdf::revision_t i=0; i<pdf->getPageCount(); i++)
		{
			pdf->changeRevision(i);
			if(!number)
			{
				// first information is just stored to number
				number=pdf->getCXref()->getNumObjects();
				continue;
			}
			CPPUNIT_ASSERT(pdf->getCXref()->getNumObjects()==number);
		}

		printf("TC05:\tolder revisions has to be readOnly\n");
		for(CPdf::revision_t i=1; i<pdf->getRevisionsCount(); i++)
		{
			pdf->changeRevision(i);
			CPPUNIT_ASSERT(pdf->getMode()==CPdf::ReadOnly);
		}

		printf("TC06:\tThe newest revision has mode same as set in getInstace\n");
		pdf->changeRevision(0);
		CPPUNIT_ASSERT(pdf->getMode()==mode);
		
		printf("TC07:\tno changes can be done in older revisions\n");
		for(CPdf::revision_t i=1; i<pdf->getRevisionsCount(); i++)
		{
			printf("\trevision=%d\n", i);
			pdf->changeRevision(i);
			
			// addIndirectProperty
			printf("\t\taddIndirectProperty\n");
			shared_ptr<IProperty> prop(CIntFactory::getInstance(1));
			TRY_READONLY_OP(pdf->addIndirectProperty(prop),"addIndirectProperty should have failed");

			// changeIndirectProperty
			printf("\t\tchangeIndirectProperty\n");
			TRY_READONLY_OP(pdf->changeIndirectProperty(prop),"changeIndirectProperty should have failed");
			
			// insertPage
			shared_ptr<CDict> pageDict(CDictFactory::getInstance());
			shared_ptr<CPage> page(CPageFactory::getInstance(pageDict));
			printf("\t\tinsertPage\n");
			TRY_READONLY_OP(pdf->insertPage(page,1), "insertPage should have failed");

			// removePage
			printf("\t\tremovePage\n");
			TRY_READONLY_OP(pdf->removePage(1), "removePage should have failed");
			
			// removeOutline TODO
		}
		
		pdf->close();
	}
#undef TRY_READONLY_OP
	
	void setUp()
	{
		// creates pdf instances for all files
		for(FileList::iterator i=fileList.begin(); i!=fileList.end(); i++)
		{
			try
			{
				CPdf * pdf=getTestCPdf(i->c_str());
				pdfs.push_back(pdf);
			}catch(PdfOpenException &e)
			{
				printf("file \"%s\" open failed. msg=", e.what());
			}
		}

	}

	void instancingTC()
	{
		printf("%s\n", __FUNCTION__);

		// checks whether getInstance works correctly
		// TODO figure out
		
	}

	void indirectPropertyTC(CPdf * pdf)
	{
	using namespace boost;
	using namespace utils;
	
		printf("%s\n", __FUNCTION__);

		printf("TC01:\taddIndirectProperty with property with no pdf test\n");
		// creates new property and adds it
		shared_ptr<IProperty> prop(CIntFactory::getInstance(1));
		IndiRef addedRef=pdf->addIndirectProperty(prop);
		
		printf("TC02:\tgetIndirectProperty knows newly added property with correct type, value, pdf and indiref\n");
		shared_ptr<IProperty> added=pdf->getIndirectProperty(addedRef);
		// type must be same
		CPPUNIT_ASSERT(added->getType()==prop->getType());
		// value must be same (we know that prop is CInt)
		PropertyEquals pe;
		CPPUNIT_ASSERT(pe(added, prop));
		CPPUNIT_ASSERT(added->getPdf()==pdf);
		CPPUNIT_ASSERT(added->getIndiRef()==addedRef);
		
		printf("TC03:\taddIndirectProperty makes deep copy and returns new reference\n");
		CPPUNIT_ASSERT(! (addedRef==prop->getIndiRef()));
		CPPUNIT_ASSERT(added!=prop);

		printf("TC04:\taddIndirectProperty with CRef from same pdf makes no change\n");
		// uses root of page tree, which has to be reference
		shared_ptr<IProperty> rootProp=pdf->getDictionary()->getProperty("Pages");
		if(isRef(rootProp))
		{
			shared_ptr<CRef> rootCRef=IProperty::getSmartCObjectPtr<CRef>(rootProp);
			addedRef=pdf->addIndirectProperty(rootCRef);
			IndiRef rootRef=getValueFromSimple<CRef, pRef, IndiRef>(rootCRef);
			CPPUNIT_ASSERT(addedRef==rootRef);
		}
		
		printf("TC05:\taddIndirectProperty with not CRef from same pdf makes new property\n");
		// uses document catalog as indirect object
		shared_ptr<CDict> indirectProp=pdf->getDictionary();
		IndiRef newIndirectProp=pdf->addIndirectProperty(indirectProp);
		// referencies must be different
		CPPUNIT_ASSERT(!(newIndirectProp==indirectProp->getIndiRef()));
		
		printf("TC06:\taddIndirectProperty with CRef from different CPdf test\n");
		// uses multiversion pdf
		// TODO

		printf("TC07:\tgetIndirectProperty with uknown reference returns CNull\n");
		// creates unknown reference from existing by adding 1 to generation
		// number
		IndiRef unknownRef=pdf->getDictionary()->getIndiRef();
		unknownRef.gen++;
		CPPUNIT_ASSERT(isNull(*pdf->getIndirectProperty(unknownRef)));

		printf("TC08:\tchangeIndirectProperty with invalid parameter should fail\n");
		// creates fake and try to use changeIndirectProperty with unknownRef
		Object obj;
		obj.initInt(1);
		shared_ptr<CInt> fakeInt(CIntFactory::getInstance(*pdf, unknownRef, obj));
		try
		{
			pdf->changeIndirectProperty(fakeInt);
			CPPUNIT_FAIL("changeIndirectProperty should have failed with faked object");
		}catch(CObjInvalidObject & e)
		{
			/* ok */
		}

		printf("TC09:\tchangeIndirectProperty with bad typed parameter should fail in\n");
		// creates integer and tries to change value of Pages field which is
		// reference
		fakeInt=shared_ptr<CInt>(CIntFactory::getInstance(*pdf, pdf->getDictionary()->getProperty("Pages")->getIndiRef(), obj));
		try
		{
			pdf->changeIndirectProperty(fakeInt);
			CPPUNIT_FAIL("changeIndirectProperty with bad typed value should have failed");
		}catch(ElementBadTypeException & e)
		{
			/* ok */
		}

		// TODO REMOVE
		IndiRef contentStream=getValueFromSimple<CRef, pRef, IndiRef>(pdf->getPage(1)->getDictionary()->getProperty("Contents"));
		{
			Object obj;
			pdf->getCXref()->fetch(contentStream.num, contentStream.gen, &obj);
			int ch;
			printf("Original stream:\n");
			while((ch=obj.getStream()->getChar())!=EOF)
				printf("%d",ch);
			printf("\nend of stream\n");
			
			// simulation without stream writer
			FILE * f=fopen("../../doc/zadani.pdf", "r+");
			Object streamObj;
			FileStream * stream=new FileStream(f, 0, gFalse, 0, &streamObj);
			stream->reset();
			XRef xref(stream);
			Object obj1;
			xref.fetch(contentStream.num, contentStream.gen, &obj1);
			printf("Original stream:\n");
			while((ch=obj1.getStream()->getChar())!=EOF)
				printf("%d",ch);
			printf("\nend of stream\n");

			/*
			printf("Cloned stream:\n");
			while((ch=clone->getChar())!=EOF)
				printf("%d",ch);
			printf("\nend of stream\n");
			*/
		}
	}

	void tearDown()
	{
		// closes all opened pdfs and removes them all from pdfs list
		// to prepare this test for reuse
		for(PdfList::iterator i=pdfs.begin(); i!=pdfs.end(); i++)
			(*i)->close();
		pdfs.clear();
	}

	void Test()
	{
		instancingTC();
		for(PdfList::iterator i=pdfs.begin(); i!=pdfs.end(); i++)
		{
			indirectPropertyTC(*i);
			pageIterationTC(*i);
			pageManipulationTC(*i);
		}
		revisionsTC();
		printf("TEST_CPDF testig finished\n");
	}
};
CPPUNIT_TEST_SUITE_REGISTRATION(TestCPdf);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestCPdf, "TEST_CPDF");

pdfobjects::CPdf * getTestCPdf(const char* filename)
{
	return CPdf::getInstance(filename, CPdf::ReadWrite);
}
