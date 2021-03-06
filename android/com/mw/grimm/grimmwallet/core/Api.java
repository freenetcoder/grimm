// Copyright 2018 The Beam Team / Copyright 2019 The Grimm Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package com.mw.grimm.grimmwallet.core;

import  com.mw.grimm.grimmwallet.core.entities.Wallet;

public class Api
{
	public native boolean isWalletInitialized(String path);	
    public native void closeWallet();
    public native boolean isWalletRunning();
	public native Wallet createWallet(String appVersion, String nodeAddr, String path, String pass, String phrases, boolean restore);
	public native Wallet openWallet(String appVersion, String nodeAddr, String path, String pass);
	public native String[] createMnemonic();
    public native String[] getDictionary();
    public native boolean checkReceiverAddress(String address);
    public native String[] getDefaultPeers();

	static 
	{
		System.loadLibrary("wallet-jni");
	}
}
