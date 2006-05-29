/** @file
 QObject wrapper around CContentStream (content stream contained in a page)<br>
 Export some functions to scripting.<br>
 See doc/user/scripting.xml or kernel documentation for more informations about these functions
 @author Martin Petricek
*/

#include "qscontentstream.h"
#include "qspdfoperator.h"
#include <pdfoperators.h>

namespace gui {

/** Construct wrapper with given CContentStream */
QSContentStream::QSContentStream(boost::shared_ptr<CContentStream> _cs,Base *_base) : QSCObject ("ContentStream",_base) {
 obj=_cs;
}

/** destructor */
QSContentStream::~QSContentStream() {
}

/** Call CContentStream::getStringRepresentation(ret); return ret */
QString QSContentStream::getText() {
 std::string text;
 obj->getStringRepresentation(text);
 return text;
}

/** get CContentStream held inside this class. Not exposed to scripting */
boost::shared_ptr<CContentStream> QSContentStream::get() {
 return obj;
}

/**
 Check validity of given PDF operator<br>
 Operator is valid if:<br>
 1. Is not NULL<br>
 2. Is in this contentstream (if checkThis is true)<br>
 @param op Operator to check
 @param checkThis Check also if operator is in this contentstream
 @return true if operator is valid, false if not.
*/
bool QSContentStream::opValid(QSPdfOperator *op,bool checkThis/*=false*/) {
 //op is NULL
 if (!op) return false;
 //shared_ptr in op is NULL (should not happen)
 assert(op->get());
 if (!op->get()) return false;
 if (checkThis) {
  //Check if operator is in correct contentstream (this contentstream)
  if (op->get()->getContentStream()!=obj.get()) return false;
 }
 //We found no errors, so we declare it valid
 return true;
}

/**
 Wrap given operator in "set color" operator and replaced it by the new operator in content stream
 @param op Operator that will be wrapped
 @param r Red component of new color
 @param g Green component of new color
 @param b Blue component of new color
 @param indicateChange If set to true (default), changes will be written to underlying stream
*/
void QSContentStream::setColor(QSPdfOperator *op,double r,double g,double b,bool indicateChange/*=true*/) {
 //First check for validity
 if (!opValid(op,true)) return;
 //Call the method
 boost::shared_ptr<PdfOperator> col=operatorSetColor(op->get(),r,g,b);
 replace(op->get(),col,indicateChange);
}

/**
 \copydoc setColor(QSPdfOperator*,double,double,double,bool) 
 QSA bugfixed version
*/
void QSContentStream::setColor(QObject *op,double r,double g,double b,bool indicateChange/*=true*/) {
 QSPdfOperator* qop=dynamic_cast<QSPdfOperator*>(op);
 if (!qop) return;
 setColor((QSPdfOperator*)qop,r,g,b,indicateChange);
}

/**
 Replace old operator oldOp with new operator newOp in this stream
 @param oldOp Old operator
 @param newOp New operator
 @param indicateChange If set to true (default), changes will be written to underlying stream
 */
void QSContentStream::replace(boost::shared_ptr<PdfOperator> oldOp,boost::shared_ptr<PdfOperator> newOp,bool indicateChange/*=true*/) {
 guiPrintDbg(debug::DBG_CRIT,"Not yet implemented");
 assert(0);
 // What is "previous and next iterator"?
 // obj->replaceOperator(oldOp,newOp,col,??,??,indicateChange);
}

/**
 Wrap given operator in "set position" operator and replaced it by the new operator in content stream
 @param op Operator that will be wrapped
 @param x new position (x coordinate)
 @param y new position (y coordinate)
 @param indicateChange If set to true (default), changes will be written to underlying stream
*/
void QSContentStream::setPosition(QSPdfOperator *op,double x,double y,bool indicateChange/*=true*/) {
 //First check for validity
 if (!opValid(op,true)) return;
 //Call the method
 Point pt(x,y);
 guiPrintDbg(debug::DBG_CRIT,"Not yet implemented (pdfobjects::setPosition)");
 assert(0);
//TODO: implementation of pdfobjects::setPosition currently doeas not exist
// boost::shared_ptr<PdfOperator> pos=pdfobjects::setPosition(op->get(),pt);
// replace(op->get(),pos,indicateChange);
}

/**
 \copydoc setPosition(QSPdfOperator*,double,double,bool) 
 QSA bugfixed version
*/
void QSContentStream::setPosition(QObject *op,double x,double y,bool indicateChange/*=true*/) {
 QSPdfOperator* qop=dynamic_cast<QSPdfOperator*>(op);
 if (!qop) return;
 setPosition((QSPdfOperator*)qop,x,y,indicateChange);
}

/** Call CContentStream::deleteOperator() */
void QSContentStream::deleteOperator(QSPdfOperator *op,bool indicateChange/*=true*/) {
 //First check for validity
 if (!opValid(op,true)) return;
 obj->deleteOperator(op->get(),indicateChange);
}

/**
 \copydoc deleteOperator(QSPdfOperator*,bool) 
 QSA bugfixed version
*/
void QSContentStream::deleteOperator(QObject *op,bool indicateChange/*=true*/) {
 QSPdfOperator* qop=dynamic_cast<QSPdfOperator*>(op);
 if (!qop) return;
 deleteOperator((QSPdfOperator*)qop,indicateChange);
}

/** Call CContentStream::insertOperator() */
void QSContentStream::insertOperator(QSPdfOperator *op,QSPdfOperator *newOp,bool indicateChange/*=true*/) {
 //First check for validity
 if (!opValid(op,this)) return;
 if (!opValid(newOp)) return;
 obj->insertOperator(op->get(),newOp->get(),indicateChange);
}

/**
 \copydoc insertOperator(QSPdfOperator*,bool) 
 QSA bugfixed version
*/
void QSContentStream::insertOperator(QObject *op,QObject *newOp,bool indicateChange/*=true*/) {
 QSPdfOperator* qop=dynamic_cast<QSPdfOperator*>(op);
 QSPdfOperator* qopNew=dynamic_cast<QSPdfOperator*>(newOp);
 if (!qop) return;
 if (!qopNew) return;
 insertOperator((QSPdfOperator*)qop,(QSPdfOperator*)qopNew,indicateChange);
}

/** Call CContentStream::saveChange() */
void QSContentStream::saveChange() {
 obj->saveChange();
}

} // namespace gui

//todo: incomplete
