#ifndef WIDGETS_H
#define WIDGETS_H

#include <QObject>
#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QComboBox>
#include <QLineEdit>
#include <QCloseEvent>
#include <QPushButton>
#include <QScrollBar>
#include <QTabBar>
#include <QGLWidget>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QSet>

#include <cstdio>
  
class IMEventFilter : public QObject
{
  Q_OBJECT
public:
  QSet<Qt::Key> keyDownSet;
  QSet<Qt::Key> keyPressedSet;
  QSet<Qt::Key> keyUpSet;

  bool eventFilter(QObject *object, QEvent *event)
  {
    if (event->type() == QEvent::KeyPress)
    {
      if (((QKeyEvent*)event)->isAutoRepeat()==false)
      {
        keyDownSet.insert((Qt::Key)((QKeyEvent*)event)->key());  
      }        
    }

    if (event->type() == QEvent::KeyRelease)
    {
      if (((QKeyEvent*)event)->isAutoRepeat()==false)
      {
        keyPressedSet.remove((Qt::Key)((QKeyEvent*)event)->key());
        keyUpSet.insert((Qt::Key)((QKeyEvent*)event)->key());
      }
    }

    return false;
  }

  void updateState()
  {        
    foreach(Qt::Key key,keyDownSet) keyPressedSet.insert(key);
    
    keyDownSet.clear();
    keyUpSet.clear();
  }

  bool keyDown(Qt::Key key)
  {
    return keyDownSet.contains(key);
  }
  
  bool keyPressed(Qt::Key key)
  {
    return keyPressedSet.contains(key);
  }

  bool keyUp(Qt::Key key)
  {
    return keyUpSet.contains(key);
  }
};


class IMWindow : public QWidget
{
  Q_OBJECT
public:
  bool closeRequest;

  IMWindow()
  {
    closeRequest = false;
  }

  void closeEvent(QCloseEvent* event)
  {
    closeRequest = true;
    event->ignore();
  };

public slots:    
  void updateState()
  {
    closeRequest = false;
  }
};

class IMButton : public QPushButton
{
  Q_OBJECT
public:
  bool buttonWasClicked;

  IMButton(const QString& text) : QPushButton(text)
  {
    buttonWasClicked = false;
    
    QObject::connect(this,SIGNAL(clicked()),
                     this,SLOT(buttonClicked()));
  }

public slots:  
  void updateState()
  {
    buttonWasClicked = false;
  }

  void buttonClicked()
  {
    buttonWasClicked = true;  
  }
};

class IMToggleButton : public QPushButton
{
  Q_OBJECT
public:
  bool buttonWasToggled;

  IMToggleButton(const QString& text) : QPushButton(text)
  {   
    setCheckable(true);
    buttonWasToggled = false;
    
    QObject::connect(this,SIGNAL(toggled(bool)),
                     this,SLOT(buttonToggled(bool)));
  }

  bool isHot() const
  {
    return hasFocus();
  }
    
public slots:  
  void updateState()
  {
    buttonWasToggled = false;
  }

  void buttonToggled(bool checked)
  {
    buttonWasToggled = true;  
  }  
};

class IMRadioButton : public QRadioButton
{
  Q_OBJECT
public:
  bool radioButtonStateHasChanged;

  IMRadioButton(const QString& text) : QRadioButton(text)
  { 
    setAutoExclusive(false);
   
    radioButtonStateHasChanged = false;
    
    QObject::connect(this,SIGNAL(toggled(bool)),
                     this,SLOT(radioButtonToggled(bool)));
  }

  bool isHot() const
  {
    return false;//hasFocus();
  }
  
public slots:  
  void updateState()
  {
    radioButtonStateHasChanged = false;
  }

  void radioButtonToggled(bool checked)
  {
    radioButtonStateHasChanged = true;  
  }    
};

class IMCheckBox : public QCheckBox
{
  Q_OBJECT
public:
  bool checkBoxStateHasChanged;

  IMCheckBox(const QString& text) : QCheckBox(text)
  {   
    checkBoxStateHasChanged = false;
    
    QObject::connect(this,SIGNAL(toggled(bool)),
                     this,SLOT(checkBoxToggled(bool)));
  }

  bool isHot() const
  {
    return hasFocus();
  }
    
public slots:  
  void updateState()
  {
    checkBoxStateHasChanged = false;
  }

  void checkBoxToggled(bool checked)
  {
    checkBoxStateHasChanged = true;  
  }  
};

class IMComboBox : public QComboBox
{
  Q_OBJECT
public:
  bool comboBoxStateHasChanged;

  IMComboBox() : QComboBox()
  {   
    comboBoxStateHasChanged = false;
    
    QObject::connect(this,SIGNAL(activated(int)),
                     this,SLOT(comboBoxChanged(int)));
                       
  }

public slots:  
  void updateState()
  {
    comboBoxStateHasChanged = false;
  }

  void comboBoxChanged(int index)
  {
    comboBoxStateHasChanged = true;  
  }  
};

class IMSlider : public QSlider
{
  Q_OBJECT
public:
  bool mouseOver;
  //bool sliderValueHasChanged;

  IMSlider() : QSlider()
  {
    mouseOver = false;
    /*
    sliderValueHasChanged = false;
    
    QObject::connect(this,SIGNAL(valueChanged(int)),
                     this,SLOT(sliderValueChanged(int)));
    */                     
  }
  
  void enterEvent(QEvent* event)
  {
    mouseOver = true;
    QSlider::enterEvent(event);
  }

  void leaveEvent(QEvent* event)
  {
    mouseOver = false;
    QSlider::leaveEvent(event);
  }

  bool isHot() const
  {
    return (mouseOver || hasFocus());
  }
    
public slots:  
  void updateState()
  {
  //  sliderValueHasChanged = false;
  }
/*
  void sliderValueChanged(int value)
  {
    sliderValueHasChanged = true;  
  }  
*/  
};

class IMScrollBar : public QScrollBar
{
  Q_OBJECT
public:
  bool mouseOver;
  //bool sliderValueHasChanged;

  IMScrollBar() : QScrollBar()
  {
    mouseOver = false;
    /*
    sliderValueHasChanged = false;

    QObject::connect(this,SIGNAL(valueChanged(int)),
                     this,SLOT(sliderValueChanged(int)));
    */
  }

  void enterEvent(QEvent* event)
  {
    mouseOver = true;
    QScrollBar::enterEvent(event);
  }

  void leaveEvent(QEvent* event)
  {
    mouseOver = false;
    QScrollBar::leaveEvent(event);
  }

  bool isHot() const
  {
    return mouseOver;// || hasFocus());
  }
    
public slots:  
  void updateState()
  {
    // sliderValueHasChanged = false;
  }
  
  /*
  void sliderValueChanged(int value)
  {
    sliderValueHasChanged = true;  
  }
  */
};

class IMSpinBox : public QSpinBox
{
  Q_OBJECT
public:
  bool mouseOver;
  bool spinBoxValueHasChanged;

  IMSpinBox() : QSpinBox()
  {
    mouseOver = false;
    spinBoxValueHasChanged = false;
    
    QObject::connect(this,SIGNAL(valueChanged(int)),
                     this,SLOT(spinBoxValueChanged(int)));
  }
  
  void enterEvent(QEvent* event)
  {
    mouseOver = true;
    QSpinBox::enterEvent(event);
  }

  void leaveEvent(QEvent* event)
  {
    mouseOver = false;
    QSpinBox::leaveEvent(event);
  }

  bool isHot() const
  {
    return (mouseOver || hasFocus());
  }
    
public slots:  
  void updateState()
  {
    spinBoxValueHasChanged = false;
  }

  void spinBoxValueChanged(int value)
  {
    spinBoxValueHasChanged = true;  
  }  
};


class IMDoubleSpinBox : public QDoubleSpinBox
{
  Q_OBJECT
public:
  bool mouseOver;
  bool spinBoxValueHasChanged;

  IMDoubleSpinBox() : QDoubleSpinBox()
  {
    mouseOver = false;
    spinBoxValueHasChanged = false;
    
    QObject::connect(this,SIGNAL(valueChanged(double)),
                     this,SLOT(spinBoxValueChanged(double)));
  }
  
  void enterEvent(QEvent* event)
  {
    mouseOver = true;
    QDoubleSpinBox::enterEvent(event);
  }

  void leaveEvent(QEvent* event)
  {
    mouseOver = false;
    QDoubleSpinBox::leaveEvent(event);
  }

  bool isHot() const
  {
    return (mouseOver || hasFocus());
  }
    
public slots:  
  void updateState()
  {
    spinBoxValueHasChanged = false;
  }

  void spinBoxValueChanged(double value)
  {
    spinBoxValueHasChanged = true;  
  }  
};

class IMLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  bool lineEditValueHasChanged;

  IMLineEdit() : QLineEdit()
  {
    lineEditValueHasChanged = false;
    
    QObject::connect(this,SIGNAL(textEdited(const QString&)),
                     this,SLOT(lineEditValueChanged(const QString&)));
  }
  
  bool isHot() const
  {
    return hasFocus();
  }
    
public slots:  
  void updateState()
  {
    lineEditValueHasChanged = false;
  }

  void lineEditValueChanged(const QString& value)
  {
    lineEditValueHasChanged = true;  
  }  
};

class IMPixmap : public QLabel
{
  Q_OBJECT
public:    
  bool widgetWasResized;
  
  int button2id[5];

  enum ButtonState
  {
    Released,Down,Pressed,Up
  };
  
  ButtonState mouseButtonStates[3];
  QPoint mousePosition;
  int wheelDelta;  
    
  IMPixmap()
  {  
    widgetWasResized = false;
    
    button2id[Qt::LeftButton] = 0;
    button2id[Qt::RightButton] = 1;
    button2id[Qt::MidButton] = 2;
    
    for(int i=0;i<3;i++) mouseButtonStates[i] = Released;
    
    mousePosition = QPoint(0,0);
    
    wheelDelta = 0;
    
    setMouseTracking(true);     
  }
  
  void resizeEvent(QResizeEvent* event)
  {
    widgetWasResized = true;
  }  

  void mousePressEvent(QMouseEvent* event) 
  {
    mouseButtonStates[button2id[event->button()]] = Down;
    mousePosition = event->pos();
    setFocus(Qt::MouseFocusReason);
  }

  void mouseReleaseEvent(QMouseEvent* event) 
  {
    mouseButtonStates[button2id[event->button()]] = Up;
    mousePosition = event->pos();
  }
  
  void mouseMoveEvent(QMouseEvent* event)
  {
    mousePosition = event->pos();
  } 
  
  void wheelEvent(QWheelEvent* event)
  {
    wheelDelta = event->delta();
  }
  
public slots:
  bool widgetResized()
  {
    return widgetWasResized;
  }

  bool mouseDown(int button)
  {
    return mouseButtonStates[button2id[button]]==Down;
  }

  bool mousePressed(int button)
  {
    return mouseButtonStates[button2id[button]]==Pressed;
  }

  bool mouseUp(int button)
  {
    return mouseButtonStates[button2id[button]]==Up;
  }

  int mouseX()
  {
    return mousePosition.x();
  }  

  int mouseY()
  {
    return mousePosition.y();
  }  
  
  int mouseWheelDelta()
  {
    return wheelDelta;
  }
  
  void updateState()
  {
    widgetWasResized = false;
        
    for(int i=0;i<3;i++)
    {
      if (mouseButtonStates[i] == Down) mouseButtonStates[i] = Pressed;
      else if (mouseButtonStates[i] == Up) mouseButtonStates[i] = Released;
    }

    wheelDelta = 0;
  }   
};

class GLContextPrivate : public QWidget
{
  Q_OBJECT
public:
  QGLWidget* glWidget;
  
  bool widgetWasResized;
  
  int button2id[5];

  enum ButtonState
  {
    Released,Down,Pressed,Up
  };
    
  ButtonState mouseButtonStates[3];  
  QPoint mousePosition;
  int wheelDelta;  
      
  GLContextPrivate()
  {
    QHBoxLayout* layout = new QHBoxLayout(this);    
    glWidget = new QGLWidget(QGLFormat::defaultFormat());
//    layout->setSizeConstraint(QLayout::SetMaximumSize);
    //glWidget->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(glWidget);
  
    widgetWasResized = false;
      
    button2id[Qt::LeftButton] = 0;
    button2id[Qt::RightButton] = 1;
    button2id[Qt::MidButton] = 2;
    
    for(int i=0;i<3;i++) mouseButtonStates[i] = Released;
    
    mousePosition = QPoint(0,0);

    wheelDelta = 0;
    
    glWidget->setMouseTracking(true);
    setMouseTracking(true);    
    //setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
  }
  
  ~GLContextPrivate()
  {
    //delete glWidget;
  }
  
  void makeCurrent()
  {
    glWidget->makeCurrent();
  }

  void doneCurrent()
  {
    glWidget->doneCurrent();
  }

  void swapBuffers()
  {
    glWidget->swapBuffers();
  }  

  void resizeEvent(QResizeEvent* event)
  {
    widgetWasResized = true;
  }  

  void mousePressEvent(QMouseEvent* event) 
  {
    mouseButtonStates[button2id[event->button()]] = Down;
    mousePosition = event->pos();
    setFocus(Qt::MouseFocusReason);
  }

  void mouseReleaseEvent(QMouseEvent* event) 
  {
    mouseButtonStates[button2id[event->button()]] = Up;
    mousePosition = event->pos();
  }
  
  void mouseMoveEvent(QMouseEvent* event)
  {
    mousePosition = event->pos();
  } 
  
  void wheelEvent(QWheelEvent* event)
  {
    wheelDelta = event->delta();
  }
  
public slots:
  bool widgetResized()
  {
    return widgetWasResized;
  }
  
  bool mouseDown(int button)
  {
    return mouseButtonStates[button2id[button]]==Down;
  }

  bool mousePressed(int button)
  {
    return mouseButtonStates[button2id[button]]==Pressed;
  }

  bool mouseUp(int button)
  {
    return mouseButtonStates[button2id[button]]==Up;
  }

  int mouseX()
  {
    return mousePosition.x();
  }  

  int mouseY()
  {
    return mousePosition.y();
  }  
  
  int mouseWheelDelta()
  {
    return wheelDelta;
  }
  
  void updateState()
  {
    widgetWasResized = false;
    
    for(int i=0;i<3;i++)
    {
      if (mouseButtonStates[i] == Down) mouseButtonStates[i] = Pressed;
      else if (mouseButtonStates[i] == Up) mouseButtonStates[i] = Released;
    }

    wheelDelta = 0;
  }
};

#endif
