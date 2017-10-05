import QtQuick 2.7
import QtWebEngine 1.5
import QtWebChannel 1.0

import QtQuick.Controls 2.2

import "../styles-uit" as StylesUIt

Item {
    id: flick

    property alias url: webViewCore.url
    property alias canGoBack: webViewCore.canGoBack
    property alias webViewCore: webViewCore
    property alias webViewCoreProfile: webViewCore.profile
    property string webViewCoreUserAgent

    property string userScriptUrl: ""
    property string urlTag: "noDownload=false";

    signal newViewRequestedCallback(var request)
    signal loadingChangedCallback(var loadRequest)

    width: parent.width

    property bool interactive: false

    StylesUIt.HifiConstants {
        id: hifi
    }

    function onLoadingChanged(loadRequest) {
        if (WebEngineView.LoadStartedStatus === loadRequest.status) {

            // Required to support clicking on "hifi://" links
            var url = loadRequest.url.toString();
            url = (url.indexOf("?") >= 0) ? url + urlTag : url + "?" + urlTag;
            if (urlHandler.canHandleUrl(url)) {
                if (urlHandler.handleUrl(url)) {
                    webViewCore.stop();
                }
            }
        }

        if (WebEngineView.LoadFailedStatus === loadRequest.status) {
            console.log(" Tablet WebEngineView failed to load url: " + loadRequest.url.toString());
        }

        if (WebEngineView.LoadSucceededStatus === loadRequest.status) {
            //disable Chromium's scroll bars
        }
    }

    WebEngineView {
        id: webViewCore

        width: parent.width
        height: parent.height

        profile: HFWebEngineProfile;
        settings.pluginsEnabled: true
        settings.touchIconsEnabled: true
        settings.allowRunningInsecureContent: true

        // creates a global EventBridge object.
        WebEngineScript {
            id: createGlobalEventBridge
            sourceCode: eventBridgeJavaScriptToInject
            injectionPoint: WebEngineScript.DocumentCreation
            worldId: WebEngineScript.MainWorld
        }

        // detects when to raise and lower virtual keyboard
        WebEngineScript {
            id: raiseAndLowerKeyboard
            injectionPoint: WebEngineScript.Deferred
            sourceUrl: resourceDirectoryUrl + "/html/raiseAndLowerKeyboard.js"
            worldId: WebEngineScript.MainWorld
        }

        // User script.
        WebEngineScript {
            id: userScript
            sourceUrl: flick.userScriptUrl
            injectionPoint: WebEngineScript.DocumentReady  // DOM ready but page load may not be finished.
            worldId: WebEngineScript.MainWorld
        }

        WebEngineScript {
            id: userCSSScript
            property string css: "\nhtml > ::-webkit-scrollbar { width: 0px; height: 0px; }"
            sourceCode: "(function() {
            var css = document.createElement('style');
            css.setAttribute('type', 'text/css');
            css.appendChild(document.createTextNode('%1'));
            document.getElementsByTagName('head')[0].appendChild(" + css + ");})()"
            injectionPoint: WebEngineScript.DocumentReady  // DOM ready but page load may not be finished.
            worldId: WebEngineScript.MainWorld
        }

        userScripts: [ createGlobalEventBridge, raiseAndLowerKeyboard, userScript, userCSSScript ]

        property string newUrl: ""
        onJavaScriptConsoleMessage: {
            console.log("Web Entity JS message: " + sourceID + " at line: " + lineNumber + " " +  message);
        }

        Component.onCompleted: {
            webChannel.registerObject("eventBridge", eventBridge);
            webChannel.registerObject("eventBridgeWrapper", eventBridgeWrapper);
//            // Ensure the JS from the web-engine makes it to our logging
//            webViewCore.javaScriptConsoleMessage.connect(function(level, message, lineNumber, sourceID) {
//                console.log("Web Entity JS message: " + sourceID + " at line: " + lineNumber + " " +  message);
//            });
            if (webViewCoreUserAgent !== undefined) {
                webViewCore.profile.httpUserAgent = webViewCoreUserAgent
            } else {
                webViewCore.profile.httpUserAgent += " (HighFidelityInterface)";
            }
        }

        onFeaturePermissionRequested: {
            grantFeaturePermission(securityOrigin, feature, true);
        }

        //disable popup
        onContextMenuRequested: {
            request.accepted = true;
        }

        onNewViewRequested: {
            newViewRequestedCallback(request)
        }

        onLoadingChanged: {
            flick.onLoadingChanged(loadRequest)
            loadingChangedCallback(loadRequest)
        }
    }

    AnimatedImage {
        //anchoring doesnt works here when changing content size
        x: flick.width/2 - width/2
        y: flick.height/2 - height/2
        source: "../../icons/loader-snake-64-w.gif"
        visible: webViewCore.loading && /^(http.*|)$/i.test(webViewCore.url.toString())
        playing: visible
        z: 10000
    }
}
