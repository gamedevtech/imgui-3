#include <cstdlib>
#include <cstdio>
#include <cassert>

#include <QObject>
#include <QSpinBox>
#include <QCheckBox>
#include <QApplication>
#include <QFrame>
#include <QFormLayout>
#include <QLabel>
#include <QSlider>
#include <QTextEdit>
#include <QString>
#include <QHash>
#include <QDockWidget>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSet>
#include <QStack>
#include <QHash>
#include <QGroupBox>
#include <QTabBar>
#include <QMenuBar>
#include <QMenu>
#include <QMetaObject>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QImage>
#include <QPixmap>
#include <QCleanlooksStyle>
#include <QPlastiqueStyle>
#include <QFileDialog>

#include <QGLFormat>
#include <QGLContext>
#include <QGLWidget>

#include "widgets.h"

#include <gui.h>

class OptsPrivate
{
public:  
  OptsPrivate() {}  

  OptsPrivate(const OptsPrivate& opts) : options(opts.options) {}

  OptsPrivate& operator=(const OptsPrivate& opts)
  {
    options = QHash<QString,QVariant>(opts.options);  
  }
  
  template<typename T> void set(QString opt,const T& value)
  {
    options.insert(opt,QVariant(value));
  }
 
  template<typename T> T get(QString opt) const
  {
    assert(options.contains(opt));    
    QVariant optValue = options.value(opt);    
    assert(optValue.canConvert<T>());
    return optValue.value<T>();
  }

  template<typename T> T get(QString opt,const T& defaultValue) const
  {
    if (options.contains(opt))
    {
      QVariant optValue = options.value(opt);      
      assert(optValue.canConvert<T>());
      return optValue.value<T>();
    }    
    return defaultValue;
  }
  
  bool isSet(const char* opt) const
  {
    return options.contains(opt);
  }
  
  void unite(const OptsPrivate& opts)
  {
    options.unite(opts.options);
  }
  
  QHash<QString,QVariant> options;
};

void applyOpts(QObject* obj,const OptsPrivate& opts)
{
  QStringList ignoreOpts;
  ignoreOpts << "align";
  ignoreOpts << "stretch";
  ignoreOpts << "gridRow" << "gridColumn" << "gridRowSpan" << "gridColumnSpan";
  ignoreOpts << "marginLeft" << "marginTop" << "marginRight" << "marginBottom";
  
  QHashIterator<QString,QVariant> it(opts.options);

  while (it.hasNext()) 
  {
    it.next();
    
    QString opt = it.key();
    
    if (ignoreOpts.contains(opt)) continue;
    
    QVariant value = it.value();
    
    if (obj->metaObject()->indexOfProperty(qPrintable(opt))!=-1)
    {
      obj->setProperty(qPrintable(opt),value);           
    }
    else
    {
      printf("Warning: invalid option \"%s\"\n!",qPrintable(opt));
    }
  }
  
  if (opts.isSet("marginLeft") && 
      opts.isSet("marginTop") && 
      opts.isSet("marginRight") && 
      opts.isSet("marginBottom"))
  {
    if (QLayout* layout = qobject_cast<QLayout*>(obj)) 
    {
      layout->setContentsMargins(opts.get<int>("marginLeft"),
                                 opts.get<int>("marginTop"),
                                 opts.get<int>("marginRight"),
                                 opts.get<int>("marginBottom"));
    }
    else if (QWidget* widget = qobject_cast<QWidget*>(obj))
    {
      widget->setContentsMargins(opts.get<int>("marginLeft"),
                                 opts.get<int>("marginTop"),
                                 opts.get<int>("marginRight"),
                                 opts.get<int>("marginBottom"));
    }
    else
    {
      printf("applyOpts FAIL!\n"); fflush(stdout);
    }    
  }      
}

Opts::Opts()
{
  opts = new OptsPrivate(); 
}

Opts::~Opts()
{ 
  delete opts; 
}

Opts::Opts(const Opts& other)
{ 
  opts = new OptsPrivate(*other.opts); 
}

Opts::Opts(const Opts& other0,const Opts& other1)
{ 
  opts = new OptsPrivate(*other0.opts); 
  opts->unite(*other1.opts);
}

Opts::Opts(const Opts& other0,const Opts& other1,const Opts& other2)
{ 
  opts = new OptsPrivate(*other0.opts);
  opts->unite(*other1.opts);
  opts->unite(*other2.opts);
}

Opts::Opts(const Opts& other0,const Opts& other1,const Opts& other2,const Opts& other3)
{ 
  opts = new OptsPrivate(*other0.opts); 
  opts->unite(*other1.opts);
  opts->unite(*other2.opts);
  opts->unite(*other3.opts);
}

Opts::Opts(const Opts& other0,const Opts& other1,const Opts& other2,const Opts& other3,const Opts& other4)
{ 
  opts = new OptsPrivate(*other0.opts); 
  opts->unite(*other1.opts);
  opts->unite(*other2.opts);
  opts->unite(*other3.opts);
  opts->unite(*other4.opts);
}

Opts& Opts::operator=(const Opts& other) 
{ 
  if (this != &other)
  {
    delete opts;
    opts = new OptsPrivate(*other.opts);  
  }  
  return *this;
}

Opts& Opts::align(int alignFlags) { opts->set("align",(Qt::Alignment)alignFlags); return *this; }

Opts& Opts::stretch(int stretch) { opts->set("stretch",stretch); return *this; }

Opts& Opts::cell(int row,int column)
{ 
  opts->set("gridRow",row); 
  opts->set("gridColumn",column); 
    
  return *this;
}

Opts& Opts::span(int rowSpan,int columnSpan)
{ 
  opts->set("gridRowSpan",rowSpan); 
  opts->set("gridColumnSpan",columnSpan); 
  return *this;
}

Opts& Opts::sizePolicy(SizePolicy horizontal,SizePolicy vertical) 
{ 
  opts->set("sizePolicy",QSizePolicy((QSizePolicy::Policy)horizontal,(QSizePolicy::Policy)vertical)); 
  return *this; 
}

Opts& Opts::minimumWidth(int width) { opts->set("minimumWidth",width); return *this; }
Opts& Opts::minimumHeight(int height) { opts->set("minimumHeight",height); return *this; }
Opts& Opts::minimumSize(int width,int height) { minimumWidth(width); minimumHeight(height); return *this; }

Opts& Opts::maximumWidth(int width) { opts->set("maximumWidth",width); return *this; }
Opts& Opts::maximumHeight(int height) { opts->set("maximumHeight",height); return *this; }
Opts& Opts::maximumSize(int width,int height) { maximumWidth(width); maximumHeight(height); return *this; }

Opts& Opts::fixedWidth(int width) { minimumWidth(width); maximumWidth(width); return *this; }
Opts& Opts::fixedHeight(int height) { minimumHeight(height); maximumHeight(height); return *this; }
Opts& Opts::fixedSize(int width,int height) { fixedWidth(width); fixedHeight(height); return *this; }

Opts& Opts::margins(int left,int top,int right,int bottom)
{ 
  opts->set("marginLeft",left); 
  opts->set("marginTop",top); 
  opts->set("marginRight",right); 
  opts->set("marginBottom",bottom); 
  return *this;
}

Opts& Opts::enabled(bool enabled) { opts->set("enabled",enabled); return *this; }

Opts& Opts::cursor(CursorShape cursor) { opts->set("cursor",QCursor((Qt::CursorShape)cursor)); return *this; }

Opts& Opts::alignText(int alignFlags) { opts->set("alignment",(Qt::Alignment)alignFlags); return *this; }

Opts& Opts::readOnly(bool readOnly) { opts->set("readOnly",readOnly); return *this; }

Opts& Opts::tracking(bool tracking) { opts->set("tracking",tracking); return *this; }
Opts& Opts::singleStep(int step) { opts->set("singleStep",step); return *this; }
Opts& Opts::singleStep(float step) { opts->set("singleStep",step); return *this; }
Opts& Opts::pageStep(int step) { opts->set("pageStep",step); return *this; }

Opts& Opts::tickInterval(int interval) { opts->set("tickInterval",interval); return *this; }
Opts& Opts::tickPosition(SliderTicks ticks) { opts->set("tickPosition",(QSlider::TickPosition)ticks); return *this; }

Opts& Opts::keyboardTracking(bool tracking) { opts->set("keyboardTracking",tracking); return *this; }
Opts& Opts::decimals(int decimals) { opts->set("decimals",decimals); return *this; }

Opts& Opts::frameShape(FrameShape shape) { opts->set("frameShape",(QFrame::Shape)shape); return *this; }
Opts& Opts::frameShadow(FrameShadow shadow) { opts->set("frameShadow",(QFrame::Shadow)shadow); return *this; }
Opts& Opts::frameLineWidth(int width) { opts->set("lineWidth",width); return *this; }
Opts& Opts::frameMidLineWidth(int width) { opts->set("midLineWidth",width); return *this; }

Opts& Opts::sizeConstraint(SizeConstraint constraint)
{ 
  opts->set("sizeConstraint",(QLayout::SizeConstraint)constraint); 
  return *this; 
}

Opts& Opts::spacing(int spacing) { opts->set("spacing",spacing); return *this; }
Opts& Opts::horizontalSpacing(int spacing) { opts->set("horizontalSpacing",spacing); return *this; }
Opts& Opts::verticalSpacing(int spacing) { opts->set("verticalSpacing",spacing); return *this; }
Opts& Opts::spacing(int hspacing,int vspacing) { horizontalSpacing(hspacing); verticalSpacing(vspacing); return *this; }

struct LayoutPosition
{
  LayoutPosition()
  {
    order = -1;
    row = -1;
    column = -1;
    rowSpan = -1;
    columnSpan = -1;
  }
  
  int order;
  
  int row;
  int column;
  int rowSpan;
  int columnSpan;
};

QApplication* app;

QHash<int,QWidget*> widgets;
QHash<int,QLayout*> layouts;

QStack<QWidget*> widgetStack;
QStack<QLayout*> layoutStack;
QStack<int>      orderStack;

QSet<QObject*> fresh;

QHash<QObject*,QLayout*> parentLayout;
QHash<QObject*,LayoutPosition> layoutPosition;

bool needsReinsert(QObject* object,QLayout* layout,const OptsPrivate& opts)
{
  assert(object->inherits("QWidget") || object->inherits("QLayout"));
  assert(layout->inherits("QBoxLayout") || layout->inherits("QGridLayout"));

  if (layout->inherits("QBoxLayout"))
  {
    // pridat kontrolu zmeny stretche a alignmentu
    if (orderStack.top()!=layoutPosition[object].order) return true;
  }
  else if (layout->inherits("QGridLayout"))
  {
    int row = opts.get<int>("gridRow",0);
    int column = opts.get<int>("gridColumn",0);
    int rowSpan = opts.get<int>("gridRowSpan",1);
    int columnSpan = opts.get<int>("gridColumnSpan",1);

    // pridat kontrolu zmeny alignmentu
    if (row!=layoutPosition[object].row) return true;
    if (column!=layoutPosition[object].column) return true;
    if (rowSpan!=layoutPosition[object].rowSpan) return true;
    if (columnSpan!=layoutPosition[object].columnSpan) return true;
  }

  return false;  
};

void insertToLayout(QObject* object,QLayout* layout,const OptsPrivate& opts)
{  
  assert(object->inherits("QWidget") || object->inherits("QLayout"));
  assert(layout->inherits("QBoxLayout") || layout->inherits("QGridLayout"));
  
  if (QBoxLayout* boxLayout = qobject_cast<QBoxLayout*>(layout)) 
  {
    int order = orderStack.top();
    int stretch = opts.get<int>("stretch",0);
    Qt::Alignment alignment = (Qt::Alignment)opts.get<int>("align",0);
    
    if (QWidget* widgetItem = qobject_cast<QWidget*>(object))
    {
      boxLayout->insertWidget(order,widgetItem,stretch,alignment);      
    }
    else if (QLayout* layoutItem = qobject_cast<QLayout*>(object))
    {
      boxLayout->insertLayout(order,layoutItem,stretch);      
    }

    // pridat stretch a alignment do layoutPosition zaznamu!!!!
    layoutPosition[object] = LayoutPosition();
    layoutPosition[object].order = order;
  }
  else if (QGridLayout* gridLayout = qobject_cast<QGridLayout*>(layout))
  {
    /*
    if (!opts.isSet("gridRow") || !opts.isSet("gridColumn"))
    {
      printf("Error: cell not set!\n");
    }
    */
    
    int row = opts.get<int>("gridRow",0);
    int column = opts.get<int>("gridColumn",0);
    int rowSpan = opts.get<int>("gridRowSpan",1);
    int columnSpan = opts.get<int>("gridColumnSpan",1);
    Qt::Alignment alignment = (Qt::Alignment)opts.get<int>("align",0);
    
    if (QWidget* widgetItem = qobject_cast<QWidget*>(object))
    {
      gridLayout->addWidget(widgetItem,row,column,rowSpan,columnSpan,alignment);    
    }
    else if (QLayout* layoutItem = qobject_cast<QLayout*>(object))
    {
      gridLayout->addLayout(layoutItem,row,column,rowSpan,columnSpan,alignment);  
    }

    // pridat alignment do layoutPosition zaznamu!!!!
    layoutPosition[object] = LayoutPosition();
    layoutPosition[object].row = row;
    layoutPosition[object].column = column;
    layoutPosition[object].rowSpan = rowSpan;
    layoutPosition[object].columnSpan = columnSpan;
  }
  
  //parentLayout[object] = layout;
}

void reparentWidget(QWidget* widget,const OptsPrivate& opts)
{ 
  assert(layoutStack.top()!=0);
  
  if (parentLayout[widget] != layoutStack.top())
  {
    printf("reparenting %s\n",qPrintable(widget->objectName()));
    
    if (parentLayout[widget]!=((QLayout*)-1)) parentLayout[widget]->removeWidget(widget);        
    widget->setParent(0); //According to Qt documentation, this shouldn't be necessary, "The layout will automatically reparent the widgets (using QWidget::setParent()) so that they are children of the widget on which the layout is installed"
    parentLayout[widget] = (QLayout*)-1;
           
    insertToLayout(widget,layoutStack.top(),opts);
    parentLayout[widget] = layoutStack.top();
  }
}

// Widgets that are inside VBox or HBox layout must appear in the same order in which their respective functions are called.
// If this ordering changed from the last call to update() then each affected widget must be reinserted to it's parentLayout at correct index
// given by the value at the top of the orderStack.
void reinsertWidget(QWidget* widget,const OptsPrivate& opts)
{
  assert(parentLayout[widget]!=0); 
  assert(parentLayout[widget]!=(QLayout*)-1);
  
  if (needsReinsert(widget,parentLayout[widget],opts))
  {
    printf("reinserting %s\n",qPrintable(widget->objectName()));
    parentLayout[widget]->removeWidget(widget);    
    insertToLayout(widget,parentLayout[widget],opts);
  }
}

void deleteLayout(QLayout* layout)
{
  printf("deleting layout %s\n",qPrintable(layout->objectName()));fflush(stdout);
  
  // QLayout si udrzuje reference na svoje sub-layouty jako potomky pomoci children mechanizmu QObjectu.
  // V momente kdy smazu layout, smaze jeho destruktor i vsechny svoje children sub-layouty,
  // a rekurzivne se tak smaze celej podstrom layoutuu zakorenenej v tomhle layoutu.
  //
  // To je sice pekny, ale ja se tohohle chovani potrebuju zbavit, protoze si chci vsechny vytvoreny layouty
  // smazat sam, rucne, v libovolnym poradi.
  //
  // Takze musim rozpojit Parent<--Child vazby mezi timhle layoutem a jeho sub-layoutama, a teprv potom
  // muzu layout smazat. Jakmile ty vazby rozpojim tak layout nema zadny potomky, takze mi v destruktoru
  // nemuze ty sub-layouty smazat, a ja si je pak muzu smazat kdy budu chtit.
  //
  // Podle dokumentace by se zdalo, ze Parent<--Child vazba se da rozpojit zavolanim parentLayout->removeItem(childLayout),
  // ale neni to tak, chlidLayout zustane child parentLayoutu. Misto toho funguje tohle: vsem childum se nastavi parent na 0   

  //printf("OK0\n"); fflush(stdout);
  for (int i = 0; i < layout->count(); ++i)   
  { 
    QObject* item = 0;
        
    if      (QLayout* layoutItem = layout->itemAt(i)->layout()) item = layoutItem;
    else if (QWidget* widgetItem = layout->itemAt(i)->widget()) item = widgetItem;
    //else if (layout->itemAt(i)->spacerItem() != 0) continue;
   
    //if (layout->itemAt(i)->spacerItem()!=0) { printf("THIS IS SPACER\n"); fflush(stdout); }
   
    assert(item!=0);
    
    if (parentLayout.contains(item) && parentLayout[item]==layout)
    {
      parentLayout[item] = (QLayout*)-1;      
    }    
  }
  //printf("OK1\n"); fflush(stdout);

  QObjectList children = layout->children();

  for (int i = 0; i < children.size(); ++i) 
  { 
    QObject* child = children.at(i);

    child->setParent(0);  
  }
  //printf("OK2\n"); fflush(stdout);
  
  ///////////////////////////////////
  if (parentLayout[layout]!=(QLayout*)-1 && parentLayout[layout]!=0)  parentLayout[layout]->removeItem(layout);
  ///////////
  //printf("OK3\n"); fflush(stdout);
  
  parentLayout.remove(layout);
  if (layoutPosition.contains(layout)) layoutPosition.remove(layout);
  fresh.remove(layout);

  layouts.remove(layout->property("id").toInt());
  //printf("OK4\n"); fflush(stdout);
    
  // Layout uz ted nema zadny potomky, takze ho vesle smazu
  delete layout;
  //printf("OK5\n"); fflush(stdout);
};

void deleteWidget(QWidget* widget)
{
  printf("deleting widget %s\n",qPrintable(widget->objectName()));fflush(stdout);
       
  if (widget->layout()!=0) deleteLayout(widget->layout());  
  
  // Dal musime rozpojit vazby mezi samotnym groupboxem a jeho child widgetama
  QObjectList children = widget->children();
  for (int i = 0; i < children.size(); ++i)
  {
    //printf("%s->children(%d)=%s\n",qPrintable(widget->objectName()),i,qPrintable(children.at(i)->objectName()));
    if (QWidget* widget = qobject_cast<QWidget*>(children.at(i))) widget->clearFocus();

    children.at(i)->setParent(0); 
  }
   
  ///////////////////////////////////
  if (parentLayout[widget]!=(QLayout*)-1) parentLayout[widget]->removeWidget(widget);
  
  /////////////////////////////////////
  
  parentLayout.remove(widget);
  if (layoutPosition.contains(widget)) layoutPosition.remove(widget);
  //if (order.contains(widget)) order.remove(widget);
  fresh.remove(widget);
  widgets.remove(widget->property("id").toInt());
  
  widget->clearFocus();
  
  //// HACK !!!!
  if (widget->inherits("GLContextPrivate"))
  {
    widget->hide();
  }
  else
  {
    delete widget;  
  }    
}

void refresh(QObject* object)
{
  fresh.insert(object);   
}

void initializeWidget(int id,QWidget* widget,const OptsPrivate& opts)
{
  widget->setObjectName(QString("%1[%2]").arg(widget->metaObject()->className()).arg(id));
  
  printf("creating %s\n",qPrintable(widget->objectName()));
 
  // this is toplevel window
  if (widgetStack.empty())
  {
    parentLayout[widget] = (QLayout*)-1;
    //order[widget] = -1;  
    widgets[id] = widget;    
    widget->setProperty("id",id);

    return;    
  }
  
  QLayout* topLayout = layoutStack.top();  
  assert(topLayout!=0);
  insertToLayout(widget,topLayout,opts);
  parentLayout[widget] = layoutStack.top();

  /*
  assert(topLayout!=0);

  if (QBoxLayout* boxLayout = qobject_cast<QBoxLayout*>(topLayout)) 
  {
    boxLayout->insertWidget(orderStack.top(),
                            widget,
                            opts.get<int>("stretch",0),
                            (Qt::Alignment)opts.get<int>("flush",0));
  }
  else if (QGridLayout* gridLayout = qobject_cast<QGridLayout*>(topLayout))
  {
    if (!opts.isSet("gridRow") || !opts.isSet("gridColumn"))
    {
      printf("Error: cell not set!\n");
    }
    
    gridLayout->addWidget(widget,
                          opts.get<int>("gridRow",0),
                          opts.get<int>("gridColumn",0),
                          opts.get<int>("gridRowSpan",1),
                          opts.get<int>("gridColumnSpan",1),
                          (Qt::Alignment)opts.get<int>("flush",0));
  }
  else
  {
    printf("initializeWidgetFail!\n"); fflush(stdout);
  }
   */  
 // ((QBoxLayout*)layoutStack.top())->insertWidget(orderStack.top(),widget);
  
  //parentLayout[widget] = layoutStack.top();
  //order[widget] = orderStack.top();  
  widgets[id] = widget;    
  widget->setProperty("id",id);
}

void finalizeWidget(QWidget* widget,const OptsPrivate& opts)
{
  // this is toplevel window
  if (widgetStack.empty())
  {
    refresh(widget);
    applyOpts(widget,opts);
    return;
  }

  reparentWidget(widget,opts);
  reinsertWidget(widget,opts);
  refresh(widget);
  
  applyOpts(widget,opts);
  
  orderStack.top() = orderStack.top()+1;  
}

template<typename T> T* processLayout(int id,const OptsPrivate& opts)
{
  assert(widgetStack.empty()==false);
  
  T* layout = 0;
  
  // REPARENT PHASE
  if (layouts.contains(id))
  {
    layout = (T*)layouts[id];
    
    if (parentLayout[layout]!=layoutStack.top())
    { 
      deleteLayout(layout); 
      layout = 0;
    }
    else if (parentLayout[layout]==0 && layoutStack.top()==0)
    {
      if (layout->parentWidget()!=widgetStack.top())
      {
        deleteLayout(layout);
        layout = 0;
      }
    }      
  } 
  // INIT / REPARENT PHASE
  if (layout==0)
  {
    layout = new T();
    layout->setObjectName(QString("%1[%2]").arg(layout->metaObject()->className()).arg(id));

    printf("creating %s\n",qPrintable(layout->objectName()));

    layout->setProperty("id",id);
    layouts[id] = layout;
    
    if (layoutStack.top()==0)
    {
      assert(orderStack.top()==0);
      
      if (widgetStack.top()->layout()!=0)
      {
        deleteLayout(widgetStack.top()->layout());
      }
      
      widgetStack.top()->setLayout(layout);
    }
    else
    {
      insertToLayout(layout,layoutStack.top(),opts);//((QBoxLayout*)layoutStack.top())->insertLayout(orderStack.top(),layout);      
    }
    
    parentLayout[layout] = layoutStack.top();
    //order[layout] = orderStack.top();
  }
  else
  {    
    if (parentLayout[layout]!=0)
    {
      // REINSERT PHASE
      if (needsReinsert(layout,layoutStack.top(),opts))
      {    
        printf("reinserting %s\n",qPrintable(layout->objectName()));
    
        parentLayout[layout]->removeItem(layout);
        layout->setParent(0);
        parentLayout[layout] = (QLayout*)-1;

        insertToLayout(layout,layoutStack.top(),opts);
        parentLayout[layout] = layoutStack.top();
      }      
    }  
  }
  
  assert(layout!=0);
   
  applyOpts(layout,opts);

  refresh(layout);
  
  orderStack.top() = orderStack.top()+1;  

  layoutStack.push(layout);  
  orderStack.push(0);
  
  return layout;
}

void HBoxLayoutBegin(int id,const Opts& opts)
{
  processLayout<QHBoxLayout>(id,*opts.opts);
}

void HBoxLayoutEnd()
{
  layoutStack.pop();
  orderStack.pop();
}

void VBoxLayoutBegin(int id,const Opts& opts)
{
  processLayout<QVBoxLayout>(id,*opts.opts);
}

void VBoxLayoutEnd()
{
  layoutStack.pop();
  orderStack.pop();
}

void GridLayoutBegin(int id,const Opts& opts)
{
  processLayout<QGridLayout>(id,*opts.opts);
}

void GridLayoutEnd()
{
  layoutStack.pop();
  orderStack.pop();
}

template<typename T> T* fetchCachedWidget(int id)
{
  if (widgets.contains(id))
  {
    QWidget* widget = widgets[id];
    
    assert(qobject_cast<T*>(widget)!=0); // probably due to id collision
    
    return (T*)widget;
  }
  else
  {
    return 0;
  }  
}

void Label(int id,const char* text,const Opts& opts)
{
  QLabel* label = fetchCachedWidget<QLabel>(id);
  
  if (label==0)
  {
    label = new QLabel(text);
    
    initializeWidget(id,label,*opts.opts);
  }
  
  label->setText(text);
  
  finalizeWidget(label,*opts.opts);
}

template<int Style> void Separator(int id,const Opts& opts)
{
  QFrame* separator = fetchCachedWidget<QFrame>(id);

  if (separator==0)
  {
    separator = new QFrame();
    
    separator->setFrameStyle(Style | QFrame::Sunken);
    
    initializeWidget(id,separator,*opts.opts);
  }
  
  finalizeWidget(separator,*opts.opts);  
}

void HSeparator(int id,const Opts& opts)
{
  Separator<QFrame::HLine>(id,opts);
}

void VSeparator(int id,const Opts& opts)
{
  Separator<QFrame::VLine>(id,opts);
}

bool Button(int id,const char* iconFileName,const char* text,const Opts& opts)
{
  IMButton* button = fetchCachedWidget<IMButton>(id);

  if (button==0)
  {
    button = new IMButton(text);    
   
    initializeWidget(id,button,*opts.opts);
  }
  
  if (QString(iconFileName).isEmpty()==false)
  {
    QIcon icon(iconFileName);
    button->setIcon(icon);
    button->setIconSize(icon.availableSizes()[0]);    
  }

  finalizeWidget(button,*opts.opts);  

  return button->buttonWasClicked;
}

bool Button(int id,const char* text,const Opts& opts)
{  
  return Button(id,"",text,opts);
}

bool ToggleButton(int id,const char* iconFileName,const char* text,bool* state,const Opts& opts)
{
  IMToggleButton* toggleButton = fetchCachedWidget<IMToggleButton>(id);

  if (toggleButton==0)
  {
    toggleButton = new IMToggleButton(text);    
   
    initializeWidget(id,toggleButton,*opts.opts);
  }
  /*
  if (QString(iconFileName).isEmpty()==false)
  {
    QIcon icon(iconFileName);
    toggleButton->setIcon(icon);
    toggleButton->setIconSize(icon.availableSizes()[0]);    
  }
  */ 
  
  if (toggleButton->isChecked()!=*state && toggleButton->isHot()==false)
  {
    toggleButton->setChecked(*state);
  }
  
  *state = toggleButton->isChecked();
      
  finalizeWidget(toggleButton,*opts.opts);  

  return toggleButton->buttonWasToggled;
}

bool CheckBox(int id,const char* text,bool* state,const Opts& opts)
{
  IMCheckBox* checkBox = fetchCachedWidget<IMCheckBox>(id);

  if (checkBox==0)
  {
    checkBox = new IMCheckBox(text);    
   
    initializeWidget(id,checkBox,*opts.opts);
  }
  /*
  if (QString(iconFileName).isEmpty()==false)
  {
    QIcon icon(iconFileName);
    checkBox->setIcon(icon);
    checkBox->setIconSize(icon.availableSizes()[0]);    
  }
  */ 
  
  if (checkBox->isChecked()!=*state && checkBox->isHot()==false)
  {
    checkBox->setChecked(*state);
  }
  
  *state = checkBox->isChecked();
      
  finalizeWidget(checkBox,*opts.opts);  

  return checkBox->checkBoxStateHasChanged;
}

template<typename T,int orientation> bool AbstractSlider(int id,int min,int max,int* value,const Opts& opts)
{
  T* slider = fetchCachedWidget<T>(id);

  if (slider==0)
  {
    slider = new T();
    slider->setOrientation((Qt::Orientation)orientation);
    initializeWidget(id,slider,*opts.opts);
  }
  
  finalizeWidget(slider,*opts.opts);  
  
  slider->setRange(min,max);
  
  if (slider->value()!=*value && slider->isHot()==false) slider->setValue(*value);
  
  *value = slider->value();
  
  return slider->sliderValueHasChanged;    
}

bool HSlider(int id,int min,int max,int* value,const Opts& opts)
{
  return AbstractSlider<IMSlider,Qt::Horizontal>(id,min,max,value,opts);
}

bool VSlider(int id,int min,int max,int* value,const Opts& opts)
{
  return AbstractSlider<IMSlider,Qt::Vertical>(id,min,max,value,opts);
}

bool HScrollBar(int id,int min,int max,int* value,const Opts& opts)
{
  return AbstractSlider<IMScrollBar,Qt::Horizontal>(id,min,max,value,opts);
}

bool VScrollBar(int id,int min,int max,int* value,const Opts& opts)
{
  return AbstractSlider<IMScrollBar,Qt::Vertical>(id,min,max,value,opts);
}

bool SpinBox(int id,int min,int max,int* value,const Opts& opts)
{
  IMSpinBox* spinBox = fetchCachedWidget<IMSpinBox>(id);

  if (spinBox==0)
  {
    spinBox = new IMSpinBox();
    initializeWidget(id,spinBox,*opts.opts);
  }
  
  finalizeWidget(spinBox,*opts.opts);  
  
  spinBox->setRange(min,max);
  
  if (spinBox->value()!=*value && spinBox->isHot()==false) spinBox->setValue(*value);
  
  *value = spinBox->value();
  
  return spinBox->spinBoxValueHasChanged;      
}

bool SpinBox(int id,float min,float max,float* value,const Opts& opts)
{
  IMDoubleSpinBox* spinBox = fetchCachedWidget<IMDoubleSpinBox>(id);

  if (spinBox==0)
  {
    spinBox = new IMDoubleSpinBox();
    initializeWidget(id,spinBox,*opts.opts);
  }
  
  finalizeWidget(spinBox,*opts.opts);  
  
  spinBox->setRange(min,max);
  
  if (spinBox->value()!=*value && spinBox->isHot()==false) spinBox->setValue(*value);
  
  *value = spinBox->value();
  
  return spinBox->spinBoxValueHasChanged;      
}

bool LineEdit(int id,int* value,const Opts& opts)
{
  IMLineEdit* lineEdit = fetchCachedWidget<IMLineEdit>(id);

  if (lineEdit==0)
  {
    lineEdit = new IMLineEdit();
    lineEdit->setValidator(new QIntValidator());
    initializeWidget(id,lineEdit,*opts.opts);
  }
  
  finalizeWidget(lineEdit,*opts.opts);  
    
  if (lineEdit->isHot()==false) lineEdit->setText(QString().setNum(*value));
  
  *value = lineEdit->text().toInt();
  
  return lineEdit->lineEditValueHasChanged;      
}

bool LineEdit(int id,float* value,const Opts& opts)
{
  IMLineEdit* lineEdit = fetchCachedWidget<IMLineEdit>(id);

  if (lineEdit==0)
  {
    lineEdit = new IMLineEdit();    
    lineEdit->setValidator(new QDoubleValidator(0));
    initializeWidget(id,lineEdit,*opts.opts);
  }
  
  finalizeWidget(lineEdit,*opts.opts);  
    
  if (lineEdit->isHot()==false) lineEdit->setText(QString().setNum(*value,'f'));
  
  *value = lineEdit->text().toFloat();
  
  return lineEdit->lineEditValueHasChanged;      
}

void Spacer(int id,const Opts& opts)
{
  QFrame* frame = fetchCachedWidget<QFrame>(id);
  
  if (frame==0)
  {
    frame = new QFrame();    
    
    if (QVBoxLayout* vboxLayout = qobject_cast<QVBoxLayout*>(layoutStack.top())) 
    {
      frame->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::MinimumExpanding);
    }
    else if (QHBoxLayout* hboxLayout = qobject_cast<QHBoxLayout*>(layoutStack.top())) 
    {
      frame->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Maximum);
    }
    
    initializeWidget(id,frame,*opts.opts);
  }

  finalizeWidget(frame,*opts.opts);  
}

void WindowBegin(int id,const char* iconFileName,const char* title,const Opts& opts)
{
  assert(widgetStack.empty()==true);

  IMWindow* window = fetchCachedWidget<IMWindow>(id);
  
  if(window==0)
  {
    window = new IMWindow();

    initializeWidget(id,window,*opts.opts);
  }
  
  window->setWindowTitle(title);
  if (QString(iconFileName).isEmpty()==false) window->setWindowIcon(QIcon(iconFileName));
  
  finalizeWidget(window,*opts.opts);

  layoutStack.push(0);  
  orderStack.push(0);
  widgetStack.push(window);    
}

void WindowEnd()
{
  widgetStack.top()->show();
  
  layoutStack.pop();
  orderStack.pop();
  widgetStack.pop();

  assert(widgetStack.empty()==true);
}

bool windowCloseRequest()
{
  assert(widgetStack.empty()==false);
  
  return ((IMWindow*)widgetStack[0])->closeRequest;  
}

void FrameBegin(int id,const Opts& opts)
{
  QFrame* frame = fetchCachedWidget<QFrame>(id);

  if (frame==0)
  {
    frame = new QFrame();
    
    initializeWidget(id,frame,*opts.opts);
  }
  
  finalizeWidget(frame,*opts.opts);

  layoutStack.push(0);  
  orderStack.push(0);
  widgetStack.push(frame);
}

void FrameEnd()
{
  layoutStack.pop();
  orderStack.pop();
  widgetStack.pop();
}

void GroupBoxBegin(int id,const char* text,const Opts& opts)
{
  QGroupBox* groupBox = fetchCachedWidget<QGroupBox>(id);

  if (groupBox==0)
  {
    groupBox = new QGroupBox(text);
    
    initializeWidget(id,groupBox,*opts.opts);
  }
  
  finalizeWidget(groupBox,*opts.opts);

  layoutStack.push(0);  
  orderStack.push(0);
  widgetStack.push(groupBox);
}

void GroupBoxEnd()
{
  layoutStack.pop();
  orderStack.pop();
  widgetStack.pop();
}

void PixmapBegin(int id,const Opts& opts)
{
  IMPixmap* pixmap = fetchCachedWidget<IMPixmap>(id);

  if (pixmap==0)
  {
    pixmap = new IMPixmap();
    
    initializeWidget(id,pixmap,*opts.opts);
  }
  
  finalizeWidget(pixmap,*opts.opts);

  layoutStack.push(0);  
  orderStack.push(0);
  widgetStack.push(pixmap);  
}

void PixmapEnd()
{
  layoutStack.pop();
  orderStack.pop();
  widgetStack.pop();  
}

void pixmapBlit(int width,int height,const unsigned char* data)
{
  ((IMPixmap*)widgetStack.top())->setPixmap(QPixmap::fromImage(QImage(data,width,height,QImage::Format_ARGB32)));  
}

int widgetWidth()
{
  assert(widgetStack.top()!=0);
  return widgetStack.top()->width();  
}

int widgetHeight()
{
  assert(widgetStack.top()!=0);
  return widgetStack.top()->height();    
}

bool mouseDown(MouseButton button)
{  
  bool down = false;
  QMetaObject::invokeMethod(widgetStack.top(),"mouseDown",Qt::DirectConnection,Q_RETURN_ARG(bool,down),Q_ARG(int,button));  
  return down;
}

bool mousePressed(MouseButton button)
{
  bool pressed = false;
  QMetaObject::invokeMethod(widgetStack.top(),"mousePressed",Qt::DirectConnection,Q_RETURN_ARG(bool,pressed),Q_ARG(int,button));  
  return pressed;
}

bool mouseUp(MouseButton button)
{
  bool up = false;
  QMetaObject::invokeMethod(widgetStack.top(),"mouseUp",Qt::DirectConnection,Q_RETURN_ARG(bool,up),Q_ARG(int,button));  
  return up;
}

int mouseX()
{
  int x = 0;
  QMetaObject::invokeMethod(widgetStack.top(),"mouseX",Qt::DirectConnection,Q_RETURN_ARG(int,x));  
  return x;
}

int mouseY()
{
  int y = 0;
  QMetaObject::invokeMethod(widgetStack.top(),"mouseY",Qt::DirectConnection,Q_RETURN_ARG(int,y));  
  return y;
}

int mouseWheelDelta()
{
  int d = 0;
  QMetaObject::invokeMethod(widgetStack.top(),"mouseWheelDelta",Qt::DirectConnection,Q_RETURN_ARG(int,d));  
  return d;
}

bool keyDown(Key key)
{
  bool down = false;
  QMetaObject::invokeMethod(widgetStack.top(),"keyDown",Qt::DirectConnection,Q_RETURN_ARG(bool,down),Q_ARG(Qt::Key,((Qt::Key)key)));  
  return down;    
}

bool keyPressed(Key key)
{
  bool pressed = false;
  QMetaObject::invokeMethod(widgetStack.top(),"keyPressed",Qt::DirectConnection,Q_RETURN_ARG(bool,pressed),Q_ARG(Qt::Key,((Qt::Key)key)));  
  return pressed;    
}

bool keyUp(Key key)
{
  bool up = false;
  QMetaObject::invokeMethod(widgetStack.top(),"keyUp",Qt::DirectConnection,Q_RETURN_ARG(bool,up),Q_ARG(Qt::Key,((Qt::Key)key)));  
  return up;    
}

void guiInit(int& argc,char** argv)
{
  app = new QApplication(argc,argv);
}

void guiInit()
{
  static int argc = 1;
  static char* argv = "foo";
  guiInit(argc,&argv);
}

void guiUpdate()
{ 
  assert(widgetStack.empty()==true);
  assert(layoutStack.empty()==true);
  assert(orderStack.empty()==true);
  
  std::vector<int> remlist;

  QHashIterator<int,QWidget*> it(widgets);
  
  while (it.hasNext()) 
  {
    it.next();
    
    int id = it.key();
    QWidget* widget = it.value();
    
    if (!fresh.contains(widget))
    {
      remlist.push_back(id);
    }
    else
    {
      QMetaObject::invokeMethod(widget,"updateState");
    }
  }
  
  for(int i=0;i<remlist.size();i++)
  {
    deleteWidget(widgets[remlist[i]]);
  }
  
  remlist.clear();
  
  QHashIterator<int,QLayout*> lit(layouts);
  
  while (lit.hasNext()) 
  {
    lit.next();
    int id = lit.key();

    if (!fresh.contains(layouts[id]))
    {
      remlist.push_back(id);
    }
  }
  
  for(int i=0;i<remlist.size();i++)
  {
    deleteLayout(layouts[remlist[i]]);
  } 
  
  fresh.clear();    
  app->processEvents();
}

void guiCleanup()
{
  std::vector<int> remlist;

  QHashIterator<int,QWidget*> it(widgets);
  
  while (it.hasNext()) 
  {
    it.next();
    int id = it.key();

    remlist.push_back(id);
  }
  
  for(int i=0;i<remlist.size();i++)
  {
    deleteWidget(widgets[remlist[i]]);
  }
  
  remlist.clear();
  
  QHashIterator<int,QLayout*> lit(layouts);
  
  while (lit.hasNext()) 
  {
    lit.next();
    int id = lit.key();

    remlist.push_back(id);
  }
  
  for(int i=0;i<remlist.size();i++)
  {
    deleteLayout(layouts[remlist[i]]);
  }   
  
  fresh.clear();
  
  orderStack.clear();
  layoutStack.clear();
  widgetStack.clear();  
  
  layoutPosition.clear();  
  
  parentLayout.clear();  
  
  widgets.clear();
  layouts.clear();
  
  delete app;
};

GLContext::GLContext()
{
  glContextPrivate = new GLContextPrivate();
}

GLContext::~GLContext()
{
  delete glContextPrivate;  
}  

void GLContext::makeCurrent()
{
  glContextPrivate->makeCurrent();
}

void GLContext::doneCurrent()
{
  glContextPrivate->doneCurrent();  
}

void GLWidgetBegin(int id,GLContext* ctx,const Opts& opts)
{
  GLContextPrivate* glWidget = NULL;
  if (widgets.contains(id))
  {
    glWidget = (GLContextPrivate*)widgets[id];
  }
  else
  {
    glWidget = ctx->glContextPrivate;
    
    initializeWidget(id,glWidget,*opts.opts);
  }
  
  finalizeWidget(glWidget,*opts.opts);
  
  if (glWidget->isVisible()==false) glWidget->show();
  
  glWidget->makeCurrent();
/*
  layoutStack.push(0);  
  orderStack.push(0);
*/  
  widgetStack.push(glWidget);  
}

void GLWidgetEnd()
{ 
  GLContextPrivate* glWidget = qobject_cast<GLContextPrivate*>(widgetStack.top());
  assert(glWidget!=0);
   
  glWidget->swapBuffers();
  glWidget->doneCurrent(); 
  
  widgetStack.pop();
}
