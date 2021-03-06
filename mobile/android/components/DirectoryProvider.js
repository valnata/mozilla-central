/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

const Cc = Components.classes;
const Ci = Components.interfaces;

Components.utils.import("resource://gre/modules/XPCOMUtils.jsm");

// -----------------------------------------------------------------------
// Directory Provider for special browser folders and files
// -----------------------------------------------------------------------

const NS_APP_CACHE_PARENT_DIR = "cachePDir";
const XRE_UPDATE_ROOT_DIR     = "UpdRootD";
const ENVVAR_UPDATE_DIR       = "UPDATES_DIRECTORY";
const WEBAPPS_DIR             = "webappsDir";

function DirectoryProvider() {}

DirectoryProvider.prototype = {
  classID: Components.ID("{ef0f7a87-c1ee-45a8-8d67-26f586e46a4b}"),
  
  QueryInterface: XPCOMUtils.generateQI([Ci.nsIDirectoryServiceProvider]),

  getFile: function(prop, persistent) {
    if (prop == NS_APP_CACHE_PARENT_DIR) {
      let dirsvc = Cc["@mozilla.org/file/directory_service;1"].getService(Ci.nsIProperties);
      let profile = dirsvc.get("ProfD", Ci.nsIFile);
      return profile;
    } else if (prop == WEBAPPS_DIR) {
      // returns the folder that should hold the webapps database file
      // For fennec we will store that in the root profile folder so that all
      // webapps can easily access it
      let dirsvc = Cc["@mozilla.org/file/directory_service;1"].getService(Ci.nsIProperties);
      let profile = dirsvc.get("ProfD", Ci.nsIFile);
      return profile.parent;
    } else if (prop == XRE_UPDATE_ROOT_DIR) {
      let env = Cc["@mozilla.org/process/environment;1"].getService(Ci.nsIEnvironment);
      if (env.exists(ENVVAR_UPDATE_DIR)) {
        let path = env.get(ENVVAR_UPDATE_DIR);
        if (path) {
          let localFile = Cc["@mozilla.org/file/local;1"].createInstance(Ci.nsILocalFile);
          localFile.initWithPath(path);
          return localFile;
        }
      }
      let dm = Cc["@mozilla.org/download-manager;1"].getService(Ci.nsIDownloadManager);
      return dm.defaultDownloadsDirectory;
    }
    
    // We are retuning null to show failure instead for throwing an error. The
    // interface is called quite a bit and throwing an error is noisy. Returning
    // null works with the way the interface is called [see bug 529077]
    return null;
  }
};

const NSGetFactory = XPCOMUtils.generateNSGetFactory([DirectoryProvider]);

