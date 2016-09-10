Unit Mouse;

Interface
Uses DOS;

var
  MouseAvailable     : Boolean;
  MouseType           : Word;
  MouseName           : String[19];


{ Interrupt $33 Mouse Functions                                               }

{-----------------------------------------------------------------------------}
{-----------------------------------------------------------------------------}

{ $00 Mouse Reset                                                             }
{                                                                             }
function MouseReset:Boolean;
{                                                                             }
{ MouseReset returns TRUE if a mouse is installed.                            }
{ MouseType = numeric value of type of mouse attached if MouseReset is TRUE.  }
{ MouseType = 2 for Microsoft Mouse, MouseType = 3 for Mouse Systems Mouse.   }
{ If MouseReset returns TRUE then the MSMOUSE internal variables have been    }
{ set as follows:                                                             }
{                                                                             }
{         internal cursor level counter       	        -1                    }
{         graphics cursor shape and hot spot            arrow and (-1,-1)     }
{         text cursor                                   inverting box         }
{         user-defined call mask                        all zeros             }
{         light pen emulation mode                      on                    }
{         horizontal mickey to pixel ratio              8 to 8                }
{         vertical mickey to pixel ratio                16 to 8               }
{         horizontal min and max cursor position limit  0 and 639             }
{         vertical min and max cursor position limit    0 and 199             }
{         double speed threshold                        64 mickeys per second }


{ $01 Show Cursor                                                             }
{                                                                             }
procedure MouseShowCursor;
{                                                                             }
{ MouseShowCursor increments the internal cursor level counter.               }
{ The mouse cursor is visible if the cursor level counter = 0.                }
{ If the cursor level counter equals zero, this procedure has no effect.      }


{ $02 Hide Cursor                                                             }
{                                                                             }
procedure MouseHideCursor;
{                                                                             }
{ MouseHideCursor decrements the internal cursor level counter                }
{ The mouse cursor is hidden if the cursor level counter < 0                  }


{ $03 Get Mouse Position and Button Status                                    }
{                                                                             }
function MouseGetButtonStatus(var MouseX,MouseY:Word):Word;
{                                                                             }
{ MouseGetButtonStatus returns the state of the mouse buttons as follows:     }
{                                                                             }
{ bit 0 = left button status                                                  }
{ bit 1 = right button status                                                 }
{ bit 2 = middle button status                                                }
{ The other bits are unused.                                                  }
{ The bits are at logic 0 if the button is up and at logic 1 if it is down.   }
{                                                                             }
{ MouseX and MouseY return the X and Y coordinates of the mouse cursor.       }


{ $04 Set Mouse Cursor Position                                               }
{                                                                             }
procedure MouseSetPosition(MouseX,MouseY:Word);
{                                                                             }
{ MouseSetPosition moves the mouse cursor to a new location.                  }
{ MouseX and MouseY should be passed the desired new cursor coordinates.      }


{ $05 Get Button Press Information                                            }
{                                                                             }
function MouseGetButtonPress(Button:Word;var Count,LastX,LastY:Word):Word;
{                                                                             }
{ MouseGetButtonPress returns button status as described in function $03.     }
{ Button is passed the value of the button on which press information is      }
{ desired (0 for the left button, 1 for the right button, 2 for the middle    }
{ button).  Count returns the number of the times the indicated button was    }
{ pressed since a reset or the last call to MouseGetButtonPress.  Count will  }
{ be in the range 0 to 32767 ($7FFF) with no overflow detection.  LastX and   }
{ LastY return the cursor coordinates at the last button press.               }


{ $06 Get Button Release Information }
{                                                                             }
function MouseGetButtonRelease(Button:Word; var Count,LastX,LastY:Word):Word;
{                                                                             }
{ MouseGetButtonRelease returns button status as described in function $03.   }
{ Button is passed the value of the button on which release information is    }
{ desired (0 for the left button, 1 for the right button, 2 for the middle    }
{ button).  Count returns the number of the times the indicated button was    }
{ released since a reset or the last call to MouseGetButtonRelease.  Count    }
{ will be in the range 0 to 32767 ($7FFF) with no overflow detection.  LastX  }
{ and LastY return the cursor coordinates at the last button release.         }


{ $07 Set Min/Max Horizontal  Position                                        }
{                                                                             }
procedure MouseSetX(MinX,MaxX:Word);
{                                                                             }
{ MouseSetX limits horizontal mouse cursor movement to the values passed to   }
{ MinX and MaxX.                                                              }


{ $08 Set Min/Max Vertical Position }
{                                                                             }
procedure MouseSetY(MinY,MaxY:Word);
{                                                                             }
{ MouseSetY limits vertical mouse cursor movement to the values passed to     }
{ MinY and MaxY.                                                              }


{ $09 Set Graphics Cursor Block }
{                                                                             }
procedure MouseSetGraphicsCursor(XHotSpot,YHotSpot:Word;CursorMap:Pointer);
{                                                                             }
{ MouseSetGraphicsCursor sets up the definition of the graphics cursor array. }
{ On CGA systems, this function sets the shape, color, and center of the      }
{ graphic cursor.  A 32 word array defines a 16 by 16 pixel cursor for 640 by }
{ 200 mode, and an 8 by 16 pixel cursor for 320 by 200 mode.  The first 16    }
{ words define the screen mask, and the second 16 words define the cursor     }
{ mask, which function as follows:                                            }
{                                                                             }
{ Screen Mask Bit       Cursor Mask Bit         Resulting Screen Bit          }
{ ---------------       ---------------         --------------------          }
{       0                      0                         0                    }
{       0                      1                         1                    }
{       1                      0                same as original bit          }
{       1                      1                inverse of original bit       }
{                                                                             }
{ In 640 by 200 mode, each pixel corresponds to one bit in each of the masks. }
{                                                                             }
{ In 320 by 200 mode, each pixel is made up of two such bits which together   }
{ define the color of the pixel.                                              }
{                                                                             }
{ On EGA systems, each pixel corresponds to one bit in each of the masks. The }
{ cursor will appear as color MaxColors.  The mask bits function somewhat     }
{ differently than on CGA systems:                                            }
{                                                                             }
{ Screen Mask Bit       Cursor Mask Bit         Resulting Screen Bit          }
{ ---------------       ---------------         --------------------          }
{       0                      0                         0                    }
{       0                      1                         1                    }
{       1                      0                same as original bit          }
{       1                      1                same as original bit          }
{                                                                             }
{ Version 5.03 of MSMOUSE.SYS does not support the VGA adapter.               }
{                                                                             }
{ The hot spot specifies the location of the mouse cursor with relation to    }
{ the graphics.  The upper left corner of the graphic cursor is (0,0).        }
{ Coordinate values increase down and to the right.                           }


{ $0A Set Text Cursor }
{                                                                             }
procedure MouseSetTextCursor(CursorSelect,ScreenMask,CursorMask:Word);
{                                                                             }
{ MouseSetTextCursor sets up the definition of the text mode cursor.          }
{ Set CursorSelect to 0 for software cursor or to 1 for hardware cursor.      }
{ For the software cursor ScreenMask and CursorMask define the character and  }
{ attribute for the cursor.  For the hardware cursor ScreenMask is the scan   }
{ line start and CursorMask is the scan line end.  For the software cursor,   }
{ the screen mask is logically ANDed with the existing character then the     }
{ cursor mask is logically XORed with the result of the AND.  The high byte   }
{ defines the attribute and the low byte defines the character.               }


{ $0B Read Mouse Motion Counters }
{                                                                             }
procedure MouseReadMotion(var XMotion,YMotion:Integer);
{                                                                             }
{ MouseReadMotion gets the raw horizontal and vertical mouse movement since   }
{ the last MouseReadMotion request.  XMotion and YMotion are relative cursor  }
{ movements with positive numbers representing movement to the right and down }
{ respectively.  Overflow of the integer range is ignored.                    }


{ $0C Set User-defined Subroutine Input Mask }
{                                                                             }
procedure MouseSetUserRoutine(EventMask:Word;UserRoutine:Pointer);
{                                                                             }
{ MouseSetUserRoutine sets up the definition of the mouse user interrupt.     }
{ It sets the address of a user-defined routine to be called when a specified }
{ mouse event occurs.  The routine will be called as an interrupt when one or }
{ more of the enabled conditions occurs.  The routine will be called with the }
{ following register values:                                                  }
{                                                                             }
{ Register      Contents                                                      }
{ --------      -------------------------------------------------             }
{    AX         event identification word (see description below)             }
{    BX         button state                                                  }
{    CX         horizontal cursor position                                    }
{    DX         vertical cursor position                                      }
{    DI         raw horizontal mouse counts                                   }
{    SI         raw vertical mouse counts                                     }
{                                                                             }
{ The event mask bits are defined as follows:                                 }
{                                                                             }
{ Mask Bit      Condition                                                     }
{ --------      -----------------------                                       }
{    0          cursor position changed                                       }
{    1          left button pressed                                           }
{    2          left button released                                          }
{    3          right button pressed                                          }
{    4          right button released                                         }
{    5          middle button pressed                                         }
{    6          middle button released                                        }
{  7-15         not used                                                      }
{                                                                             }
{ A bit value of 1 will cause the the routine to be called when the specified }
{ event occurs.                                                               }


{ $0D Light Pen Emulation Mode ON }
{                                                                             }
procedure MouseLightPenOn;
{                                                                             }
{ MouseLightPenOn enables light pen emulation.  If any mouse button is down   }
{ then the pen is down and if all mouse buttons are up then the pen is off    }
{ screen.  The cursor position is saved for the next pen function when any    }
{ mouse button is pressed.                                                    }


{ $0E Light Pen Emulation Mode OFF }
{                                                                             }
procedure MouseLightPenOff;
{                                                                             }
{ MouseLightPenOff disables light pen emulation.                              }


{ $0F Set Mickey/Pixel Ratio }
{                                                                             }
procedure MouseSetMickeyToPixel(Horizontal,Vertical:Word);
{                                                                             }
{ MouseSetMickeyToPixel defines the relationship between physical mouse       }
{ movement and mouse cursor movement.  Horizontal and Vertical specify the    }
{ number of mickeys required to move the mouse cursor 8 pixels.  The values   }
{ passed must be in the range 1 thuough 32767.                                }


{ $10 Protect Cursor Position }
{                                                                             }
procedure MouseProtectCursor(Left,Top,Right,Bottom:Word);
{                                                                             }
{ MouseProtectCursor sets up a protected area of the screen.  If the cursor   }
{ moves into this area, an implicit MouseHideCursor will be performed.        }


{ $12 Set Large Graphics Cursor Block }
{                                                                             }
function MouseSetLargeGraphicsCursor(XSize,YSize,XHot,YHot:Byte;
                                     CursorMap:Pointer):Boolean;
{                                                                             }
{ MouseSetLargeGraphicsCursor sets up a cursor in a manner described for      }
{ function $09.  The cursor size is defined by Xsize (cursor width in words)  }
{ and Ysize (cursor height in pixels).  The actual cursor width depends on    }
{ the number of bits needed to define the pixel color.  The maximum cursor    }
{ size allowed is defined by the equation:                                    }
{                                                                             }
{ ((XSize*2)+1)*YSize <= 144                                                  }
{                                                                             }
{ The definition of the cursor map is the same as for function $09.           }
{                                                                             }
{ The function returns True if the large graphics cursor is available, False  }
{ if it is not available.                                                     }


{ $13 Set Double Speed Treshold }
{                                                                             }
procedure MouseSetDoubleSpeed(Threshold:Word);
{                                                                             }
{ This procedure sets the threshold (in mickeys per second) at which cursor   }
{ movement is doubled.  To disable the speed doubling action, set the         }
{ threshold to 32767 ($7FFF) mickeys per second.                              }


{ $42 Query MSMOUSE for Storage Requirements }
{                                                                             }
function MouseQueryStorage(var Space:Word):Word;
{                                                                             }
{ MouseQueryStorage returns the availability status of the mouse state        }
{ storage and restoration functions.  If the return value is -1, then the     }
{ storage functions are available and Space is the number of bytes required   }
{ to store the mouse state.  If the return value is 66, then the functions    }
{ are not available.  If the return value is 0, then MSMOUSE.SYS is not       }
{ installed (MouseReset is the prefered method of determining this).          }


{ $50 Save MSMOUSE State }
{                                                                             }
function MouseSaveState(Space:Word;SaveSpace:Pointer):Boolean;
{                                                                             }
{ MouseSaveState saves the current mouse setup.  The return value is True if  }
{ Space is large enough to hold the mouse state, and Flase if it is not.      }
{ MouseQueryStorage should always be called first to determine mouse storage  }
{ requirements.                                                               }


{ $52 Restore MSMOUSE State }
{                                                                             }
function MouseRestoreState(Space:Word;SaveSpace:Pointer):Boolean;
{                                                                             }
{ MouseRestoreState restores a previosly saved mouse setup.  The return value }
{ is True if Space is large enough to hold the mouse state, and False if it   }
{ is not.                                                                     }

{-----------------------------------------------------------------------------}
{-----------------------------------------------------------------------------}

implementation

{-----------------------------------------------------------------------------}
{-----------------------------------------------------------------------------}

const

  IRET                = $CF;

var

  Mouse_Reg           : Registers;
  MouseServicePointer : ^Byte;
  MouseService        : Byte;
  PreviousExit,
  PreviousInt09,      { Keyboard Hardware Interrupt  (IRQ1)  } { Save any     }
  PreviousInt0A,      { Possible Bus Mouse Interrupt (IRQ2)  } { interrupt    }
  PreviousInt0B,      { COM2: and COM4: Interrupt    (IRQ3)  } { vectors that }
  PreviousInt0C,      { COM1: and COM3: Interrupt    (IRQ4)  } { may be       }
  PreviousInt0D,      { Possible Bus Mouse Interrupt (IRQ5)  } { modified     }
  PreviousInt0F,      { Possible Bus Mouse Interrupt (IRQ7)  } { while using  }
  PreviousInt10,      { Video I/O Interrupt                  } { the mouse.   }
  PreviousInt16,      { Keyboard I/O Interrupt               }
  PreviousInt72,      { Possible Bus Mouse Interrupt (IRQ10) }
  PreviousInt73,      { Possible Bus Mouse Interrupt (IRQ11) }
  PreviousInt74,      { Possible Bus Mouse Interrupt (IRQ12) }
  PreviousInt77,      { Possible Bus Mouse Interrupt (IRQ15) }
  MouseInterrupt      : Pointer;

{-----------------------------------------------------------------------------}
{-----------------------------------------------------------------------------}

{ Interrupt $33 Mouse Functions                                               }

{-----------------------------------------------------------------------------}
{-----------------------------------------------------------------------------}

{ $00 Mouse Reset }

function MouseReset:Boolean;
begin
  GetIntVec($33,MouseInterrupt);
  MouseServicePointer:=MouseInterrupt;
  MouseService:=MouseServicePointer^;
  If (MouseInterrupt <> nil) and (MouseService <> IRET) then
  With Mouse_Reg do
  begin
    Ax:=$00;
    Intr($33,Mouse_Reg);
    MouseType:=Bx;
    MouseReset:=(Ax = $FFFF);
  end
  else MouseReset:=False;
end;

{-----------------------------------------------------------------------------}

{ $01 Show Cursor }

procedure MouseShowCursor;
begin
  Mouse_Reg.Ax:=$01;
  Intr($33,Mouse_Reg);
end;

{-----------------------------------------------------------------------------}

{ $02 Hide Cursor }

procedure MouseHideCursor;
begin
  Mouse_Reg.Ax:=$02;
  Intr($33,Mouse_Reg);
end;

{-----------------------------------------------------------------------------}

{ $03 Get Mouse Position and Button Status }

function MouseGetButtonStatus(var MouseX,MouseY:Word):Word;
begin
  With Mouse_Reg do
  begin
    Ax:=$03;
    Intr($33,Mouse_Reg);
    MouseX:=Cx;
    MouseY:=Dx;
    MouseGetButtonStatus:=Bx;
  end;
end;

{-----------------------------------------------------------------------------}

{ $04 Set Mouse Cursor Position }

procedure MouseSetPosition(MouseX,MouseY:Word);
begin
  With Mouse_Reg do
  begin
    Ax:=$04;
    Bx:=MouseX;
    Cx:=MouseY;
  end;
  Intr($33,Mouse_Reg);
end;

{-----------------------------------------------------------------------------}

{ $05 Get Button Press Information }

function MouseGetButtonPress(Button:Word;var Count,LastX,LastY:Word):Word;
begin
  With Mouse_Reg do
  begin
    Ax:=$05;
    Bx:=Button;
    Intr($33,Mouse_Reg);
    LastX:=Cx;
    LastY:=Dx;
    Count:=Bx;
    MouseGetButtonPress:=Ax;
  end;
end;

{-----------------------------------------------------------------------------}

{ $06 Get Button Release Information }

function MouseGetButtonRelease(Button:Word; var Count,LastX,LastY:Word):Word;
begin
  With Mouse_Reg do
  begin
    Ax:=$06;
    Bx:=Button;
    Intr($33,Mouse_Reg);
    LastX:=Cx;
    LastY:=Dx;
    Count:=Bx;
    MouseGetButtonRelease:=Ax;
  end;
end;

{-----------------------------------------------------------------------------}

{ $07 Set Min/Max Horizontal  Position }

procedure MouseSetX(MinX,MaxX:Word);
begin
  With Mouse_Reg do
  begin
    Ax:=$07;
    Cx:=MinX;
    Dx:=MaxX;
  end;
  Intr($33,Mouse_Reg);
end;

{-----------------------------------------------------------------------------}

{ $08 Set Min/Max Vertical Position }

procedure MouseSetY(MinY,MaxY:Word);
begin
  With Mouse_Reg do
  begin
    Ax:=$08;
    Cx:=MinY;
    Dx:=MaxY;
  end;
  Intr($33,Mouse_Reg);
end;

{-----------------------------------------------------------------------------}

{ $09 Set Graphics Cursor Block }

procedure MouseSetGraphicsCursor(XHotSpot,YHotSpot:Word;CursorMap:Pointer);
begin
  With Mouse_Reg do
  begin
    Ax:=$09;
    Bx:=XHotSpot;
    Cx:=YHotSpot;
    Es:=Seg(CursorMap^);
    Dx:=Ofs(CursorMap^);
  end;
  Intr($33,Mouse_Reg);
end;

{-----------------------------------------------------------------------------}

{ $0A Set Text Cursor }

procedure MouseSetTextCursor(CursorSelect,ScreenMask,CursorMask:Word);
begin
  With Mouse_Reg do
  begin
    Ax:=$0A;
    Bx:=CursorSelect;
    Cx:=ScreenMask;
    Dx:=CursorMask;
  end;
  Intr($33,Mouse_Reg);
end;

{-----------------------------------------------------------------------------}

{ $0B Read Mouse Motion Counters }

procedure MouseReadMotion(var XMotion,YMotion:Integer);
begin
  With Mouse_Reg do
  begin
    Ax:=$0B;
    Intr($33,Mouse_Reg);
    XMotion:=Cx;
    YMotion:=Dx;
  end;
end;

{-----------------------------------------------------------------------------}

{ $0C Set User-defined Subroutine Input Mask }

procedure MouseSetUserRoutine(EventMask:Word;UserRoutine:Pointer);
begin
  With Mouse_Reg do
  begin
    Ax:=$0C;
    Cx:=EventMask;
    Es:=Seg(UserRoutine^);
    Dx:=Ofs(UserRoutine^);
  end;
  Intr($33,Mouse_Reg);
end;

{-----------------------------------------------------------------------------}

{ $0D Light Pen Emulation Mode ON }

procedure MouseLightPenOn;
begin
  Mouse_Reg.Ax:=$0D;
  Intr($33,Mouse_Reg);
end;

{-----------------------------------------------------------------------------}

{ $0E Light Pen Emulation Mode OFF }

procedure MouseLightPenOff;
begin
  Mouse_Reg.Ax:=$0E;
  Intr($33,Mouse_Reg);
end;

{-----------------------------------------------------------------------------}

{ $0F Set Mickey/Pixel Ratio }

procedure MouseSetMickeyToPixel(Horizontal,Vertical:Word);
begin
  With Mouse_Reg do
  begin
    Ax:=$0F;
    Cx:=Horizontal;
    Dx:=Vertical;
  end;
  Intr($33,Mouse_Reg)
end;

{-----------------------------------------------------------------------------}

{ $10 Protect Cursor Position }

procedure MouseProtectCursor(Left,Top,Right,Bottom:Word);
begin
  With Mouse_Reg do
  begin
    Ax:=$10;
    Cx:=Left;
    Dx:=Top;
    Si:=Right;
    Di:=Bottom;
  end;
  Intr($33,Mouse_Reg)
end;

{-----------------------------------------------------------------------------}

{ $12 Set Large Graphics Cursor Block }

function MouseSetLargeGraphicsCursor(XSize,YSize,XHot,YHot:Byte;
                                     CursorMap:Pointer):Boolean;
begin
  With Mouse_Reg do
  begin
    Ax:=$12;
    Bl:=XHot;
    Bh:=XSize;
    Cl:=YHot;
    Ch:=YSize;
    Es:=Seg(CursorMap^);
    Dx:=Ofs(CursorMap^);
    Intr($33,Mouse_Reg);
    MouseSetLargeGraphicsCursor:=(Ax = $FFFF);
  end;
end;

{-----------------------------------------------------------------------------}

{ $13 Set Double Speed Treshold }

procedure MouseSetDoubleSpeed(Threshold:Word);
begin
  With Mouse_Reg do
  begin
    Ax:=$13;
    Dx:=Threshold;
  end;
  Intr($33,Mouse_Reg)
end;

{-----------------------------------------------------------------------------}

{ $42 Query MSMOUSE for Storage Requirements }

function MouseQueryStorage(var Space:Word):Word;
begin
  With Mouse_Reg do
  begin
    Ax:=$42;
    Intr($33,Mouse_Reg);
    Space:=Bx;
    MouseQueryStorage:=Ax;
  end;
end;

{-----------------------------------------------------------------------------}

{ $50 Save MSMOUSE State }

function MouseSaveState(Space:Word;SaveSpace:Pointer):Boolean;
begin
  With Mouse_Reg do
  begin
    Ax:=$50;
    Bx:=Space;
    Es:=Seg(SaveSpace^);
    Dx:=Ofs(SaveSpace^);
    Intr($33,Mouse_Reg);
    MouseSaveState:=(Ax = $FFFF);
  end;
end;

{-----------------------------------------------------------------------------}

{ $52 Restore MSMOUSE State }

function MouseRestoreState(Space:Word;SaveSpace:Pointer):Boolean;
begin
  With Mouse_Reg do
  begin
    Ax:=$52;
    Bx:=Space;
    Es:=Seg(SaveSpace^);
    Dx:=Ofs(SaveSpace^);
    Intr($33,Mouse_Reg);
    MouseRestoreState:=(Ax = $FFFF);
  end;
end;

{-----------------------------------------------------------------------------}

{$F+}procedure MouseExitProc;{$F-}                { Mouse Unit Exit Procedure }
begin
  SetIntVec($09,PreviousInt09);
  SetIntVec($0A,PreviousInt0A);
  SetIntVec($0B,PreviousInt0B);
  SetIntVec($0C,PreviousInt0C);
  SetIntVec($0D,PreviousInt0D);
  SetIntVec($0F,PreviousInt0F);
  SetIntVec($10,PreviousInt10);
  SetIntVec($16,PreviousInt16);
  SetIntVec($72,PreviousInt72);
  SetIntVec($73,PreviousInt73);
  SetIntVec($74,PreviousInt74);
  SetIntVec($77,PreviousInt77);
  ExitProc:=PreviousExit;
end;

{-----------------------------------------------------------------------------}

{ Mouse Unit Initialization Procedure}

begin
  GetIntVec($09,PreviousInt09);
  GetIntVec($0A,PreviousInt0A);
  GetIntVec($0B,PreviousInt0B);
  GetIntVec($0C,PreviousInt0C);
  GetIntVec($0D,PreviousInt0D);
  GetIntVec($0F,PreviousInt0F);
  GetIntVec($10,PreviousInt10);
  GetIntVec($16,PreviousInt16);
  GetIntVec($72,PreviousInt72);
  GetIntVec($73,PreviousInt73);
  GetIntVec($74,PreviousInt74);
  GetIntVec($77,PreviousInt77);
  PreviousExit:=ExitProc;
  ExitProc:=@MouseExitProc;
  MouseAvailable:=MouseReset;
  mousename:='No Mouse Attached';
  If MouseAvailable then case MouseType of
   2:MouseName:='Microsoft Mouse';
   3:MouseName:='Mouse Systems Mouse';
  else
   MouseName:='Generic Mouse';
  end;
end.

{-----------------------------------------------------------------------------}
{-----------------------------------------------------------------------------}
