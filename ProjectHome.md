C++ IMGUI toolkit based on Qt

<table>
<tr>
<td>
<pre><code>#include &lt;gui.h&gt;
#include &lt;stdio.h&gt;

#define ID __COUNTER__

int main(int argc,char** argv) {
  guiInit(argc,argv);
  while(1) {
    WindowBegin(ID,"Hello!");
      if (windowCloseRequest()) break;
      VBoxLayoutBegin(ID);
        Label(ID,"Hello World!");
        if (Button(ID,"Click me!")) {
          printf("HELLO WORLD\n");
        }
      VBoxLayoutEnd();
    WindowEnd();
    guiUpdate();
  }
  guiCleanup();
  return 0;
}
</code></pre>
</td>
<td valign='top'>
<img src='http://imgui.googlecode.com/files/imgui_hello_example2.png' />
</td>
</tr>
</table>