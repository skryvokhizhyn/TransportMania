
package com.chilloutsoft.transportmania;

import org.libsdl.app.SDLActivity;

public class TransportManiaActivity extends SDLActivity
{
    static {
        System.loadLibrary("TransportManiaNative");
    }
}
