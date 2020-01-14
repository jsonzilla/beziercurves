# Simple GUI Template

```ps
> cmake -G "Visual Studio 16 2019" ../src
```

```sh
> cmake -G "Unix Makefiles" ../src
> make 
```


# Plataform error
If runnig you get a error for missing dlls
Check the path to 
* Qt5Core.dll                                                           
* Qt5Cored.dll                                                          
* Qt5Gui.dll                                                            
* Qt5Guid.dll                                                           
* Qt5Widgets.dll                                                        
* Qt5Widgetsd.dll 
* platforms\qwindows.dll
* platforms\qwindowsd.dll
