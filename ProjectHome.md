C++ IMGUI toolkit based on Qt

<table>
<tr>
<td>
<pre><code>#include &lt;gui.h&gt;<br>
#include &lt;stdio.h&gt;<br>
<br>
#define ID __COUNTER__<br>
<br>
int main(int argc,char** argv) {<br>
  guiInit(argc,argv);<br>
  while(1) {<br>
    WindowBegin(ID,"Hello!");<br>
      if (windowCloseRequest()) break;<br>
      VBoxLayoutBegin(ID);<br>
        Label(ID,"Hello World!");<br>
        if (Button(ID,"Click me!")) {<br>
          printf("HELLO WORLD\n");<br>
        }<br>
      VBoxLayoutEnd();<br>
    WindowEnd();<br>
    guiUpdate();<br>
  }<br>
  guiCleanup();<br>
  return 0;<br>
}<br>
</code></pre>
</td>
<td valign='top'>
<img src='http://imgui.googlecode.com/files/imgui_hello_example2.png' />
</td>
</tr>
</table>