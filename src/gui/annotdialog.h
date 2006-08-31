#ifndef __ANNOTDIALOG_H__
#define __ANNOTDIALOG_H__

#include <qvariant.h>
#include <qdialog.h>
#include "qspage.h"
#include "kernel/static.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QTabWidget;
class QWidget;
class QGroupBox;
class QCheckBox;
class QLabel;
class QLineEdit;
class QComboBox;
class QFrame;

namespace gui {

class AnnotDialog : public QDialog
{
    Q_OBJECT

private:
    Rectangle rect;
    QSPage & page;
public:
    AnnotDialog(QSPage & p, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AnnotDialog();

    QPushButton* okBtn;
    QPushButton* pushButton11;
    QTabWidget* tabWidget3;
    QWidget* tab;
    QGroupBox* groupBox1;
    QCheckBox* open;
    QLabel* textLabel2;
    QLineEdit* textContent;
    QLabel* textLabel3;
    QComboBox* iconName;
    QLabel* textLabel4_2;
    QComboBox* state;
    QLabel* textLabel5_2;
    QComboBox* stateModel;
    QWidget* tab_2;
    QFrame* frame3;
    QLabel* textLabel2_3;
    QLineEdit* linkContent;
    QLabel* textLabel1;
    QLineEdit* destination;
    QLabel* textLabel2_2;
    QComboBox* highLight;

    void setRectangle(double xl, double yl, double width, double height);

public slots:
    virtual void fillStates( const QString & current );
    virtual void createAnnot();

protected:
    QGridLayout* AnnotDialogLayout;
    QHBoxLayout* layout8;
    QSpacerItem* spacer8;
    QSpacerItem* spacer7;
    QSpacerItem* spacer9;
    QGridLayout* tabLayout;
    QGridLayout* groupBox1Layout;
    QVBoxLayout* layout21_2;
    QHBoxLayout* layout16_2;
    QHBoxLayout* layout18_2;
    QHBoxLayout* layout19_2;
    QHBoxLayout* layout20_2;
    QGridLayout* tabLayout_2;
    QGridLayout* frame3Layout;
    QVBoxLayout* layout17;
    QSpacerItem* spacer4;
    QVBoxLayout* layout12;
    QHBoxLayout* layout16_2_2;
    QHBoxLayout* layout8_2;
    QHBoxLayout* layout9;

    void createTextAnnot();
    void createLinkAnnot();

protected slots:
    virtual void languageChange();

};

} // namespace gui

#endif
