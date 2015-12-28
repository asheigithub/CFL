/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.app;

import android.app.NativeActivity;
import android.widget.TextView;
import android.os.Bundle;
import android.view.KeyEvent;
import android.widget.Toast;
import android.view.Gravity;
import android.os.Looper;
import android.os.Handler;
public class CFLActivity extends NativeActivity
{
	protected void  onStart() 
	{
		super.onStart();
		activityonStart();
	}
	
	public native int activityonStart();
	
	static {
		System.loadLibrary("crystax");
        System.loadLibrary("TestEntry");
    }
	
}
