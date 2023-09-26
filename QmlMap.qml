

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtLocation 6.5
import QtPositioning 6.5
//import QGeoCoordinate

Rectangle {
    visible: true
    width: 800
    height: 600
    /*
        property double latitude: 17.43611269985985
        property double longitude: 78.36778679514909
    property var destination: QtPositioning.coordinate(17.4392, 78.3754);
    property var currentLocation: QtPositioning.coordinate(17.43611269985985, 78.36778679514909);
//     property variant coordinateType: QtPositioning.coordinate(0, 0)
    property var iniPos: QtPositioning.coordinate(17.43611269985985, 78.36778679514909)
    */
    property var defaultCoordinate: QtPositioning.coordinate(0, 0); // Default coordi


    property double startLat:0
    property double startLon:0
    property double endLat:0
    property double endLon:0

    property real remainingDistance:0;
    property real dp: 10 // Define dp as a property with a default value of 10
//    property Component locationmarker:maker
    Plugin {
        id: mapPlugin
        name: "osm"


        PluginParameter {
            name: "osm.mapping.providersrepository.disabled"
            value: "true"
        }

        PluginParameter {
            name: "osm.mapping.providersrepository.address"
            value: "http://maps-redirect.qt.io/osm/6.5/"
        }

    }



    RouteQuery{
        id:routeQuery
    }

    Map {
        id: mapview
        plugin: mapPlugin
        anchors.fill: parent
        zoomLevel: 13
        center: QtPositioning.coordinate(17.3850, 78.4867)
        activeMapType: supportedMapTypes[0];   // 2 is also good(white map)
//        activeMapType: supportedMapTypes.SatelliteMapNight;

        GeocodeModel {
            id: geocodeModel
            plugin: mapPlugin
            autoUpdate: false
        }
        /*
         MapCircle {
             center: QtPositioning.coordinate(startLat0, startLon);
             radius: 1.0
             border.width: 15
             border.color: "blue"
         }

         MapCircle {
                     center: QtPositioning.coordinate(endLat, endLon) // Coordinates for the second circle
                     radius: 1.0 // Adjust the radius as needed
                     border.width: 15
                     border.color: "red"
         }*/


        PinchHandler {
            id: pinch
            target: null
            onActiveChanged: if (active) {
                                 mapview.startCentroid = mapview.toCoordinate(pinch.centroid.position, false)
                             }
            onScaleChanged: (delta) => {
                                mapview.zoomLevel += Math.log2(delta)
                                mapview.alignCoordinateToPoint(mapview.startCentroid, pinch.centroid.position)
                            }
            onRotationChanged: (delta) => {
                                   mapview.bearing -= delta
                                   mapview.alignCoordinateToPoint(mapview.startCentroid, pinch.centroid.position)
                               }
            grabPermissions: PointerHandler.TakeOverForbidden
        }
        WheelHandler {
            id: wheel
            // workaround for QTBUG-87646 / QTBUG-112394 / QTBUG-112432:
            // Magic Mouse pretends to be a trackpad but doesn't work with PinchHandler
            // and we don't yet distinguish mice and trackpads on Wayland either
            acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                             ? PointerDevice.Mouse | PointerDevice.TouchPad
                             : PointerDevice.Mouse
            rotationScale: 1/120
            property: "zoomLevel"
        }


        DragHandler {
            id: drag
            target: null
            onTranslationChanged: (delta) => mapview.pan(-delta.x, -delta.y)
        }
        Shortcut {
            enabled: mapview.zoomLevel < mapview.maximumZoomLevel
            sequence: StandardKey.ZoomIn
            onActivated: mapview.zoomLevel = Math.round(mapview.zoomLevel + 1)
        }
        Shortcut {
            enabled: mapview.zoomLevel > mapview.minimumZoomLevel
            sequence: StandardKey.ZoomOut
            onActivated: mapview.zoomLevel = Math.round(mapview.zoomLevel - 1)
        }


        MapQuickItem {
            // ... (Same marker configuration as in the previous example)
            id: marker
            anchorPoint.x: sourceItem.width / 2
            anchorPoint.y: sourceItem.height
            //            sourceItemSize: Qt.size(32, 32) // You can adjust the size as needed
            sourceItem: Column {
                Image {
                    id: image; source: "marker.png"
                    width: 32    // Make sure the image fits within the specified size
                    height: 34 // Make sure the image fits within the specified size
                }
                //                Text { text: title; font.bold: true }
            }
            coordinate: QtPositioning.coordinate(startLat,startLon) // Display current location

        }

        Text {

            text: "Remaining Distance: " + (remainingDistance / 1000).toFixed(1) + " km";
            font.bold: true;
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
                bottomMargin: 20

            }
        }
        //         center:  QtPositioning.coordinate(startLat,startLon)
        MapRoute {
            id: routeItem
            line.width: 5
            //            route: routeModel.get(0)
            line.color: "blue"
        }

        // Add markers for the start and end points


        MapQuickItem {
            id: endLocation


            coordinate: routeModel.count > 0 && routeModel.get(0).startCoordinate ? routeModel.get(0).startCoordinate : defaultCoordinate;

        }
        MapQuickItem {
            id: endMarker
            anchorPoint.x: sourceItem.width / 2
            anchorPoint.y: sourceItem.height

            sourceItem: Column {
                Image {
                    id: image2; source: "end_marker.png"
                    width: 32 // Make sure the image fits within the specified size
                    height: 34 // Make sure the image fits within the specified size


                }
                //                Text { text: title; font.bold: true }
            }

            coordinate: QtPositioning.coordinate(endLat,endLon) // Display current location

            //            coordinate: routeModel.count > 0 && routeModel.get(0).startCoordinate ? routeModel.get(0).startCoordinate : defaultCoordinate;

        }


    }

    Column {
        // Input fields for latitude and longitude
        Label {
            font.bold: true
            text: "Start Destination"
            font.pixelSize:18
        }
        Row {
            spacing:10
            TextField {

                placeholderText: "Latitude"
                onTextChanged:{
                    startLat = parseFloat(text)
                    mapview.center = QtPositioning.coordinate(startLat, startLon); // Update map center
                }
            }
            //                rightPadding: 15
            TextField {
                placeholderText: "Longitude"
                onTextChanged: {
                    startLon = parseFloat(text)
                    mapview.center = QtPositioning.coordinate(startLat, startLon); // Update map center
                }
            }
        }

        Label {
            font.bold: true
            text: "End Destination"
            font.pixelSize:18
        }
        Row {
            spacing:10
            TextField {
                placeholderText: "Latitude"
                onTextChanged: endLat = parseFloat(text)
            }
            TextField {
                placeholderText: "Longitude"
                onTextChanged: endLon = parseFloat(text)
            }
        }
        spacing: 3

        Button {

            text: "Plan Route"
            background: Item{Rectangle{color: "#ffc800"
                    anchors.fill: parent}}

            onClicked: {
                // When the user clicks this button, update the route
                planRoute();
            }
            //            Layout.alignment: Qt.AlignHCenter // Align the button to the center horizontally
        }
    }



    MapQuickItem {
        id: endmarker
        anchorPoint.x: sourceItem.width / 2
        anchorPoint.y: sourceItem.height
        sourceItem: Image {
            source: "end_marker.png" // Replace with your marker image
        }
        coordinate: routeModel.count > 0 && routeModel.get(0).startCoordinate ? routeModel.get(0).startCoordinate : defaultCoordinate;

    }





    RouteModel {
        id: routeModel
        plugin: mapPlugin
        query: routeQuery

        Component.onCompleted: {
            // Define your route waypoints
            routeQuery.addWaypoint(QtPositioning.coordinate(startLat, startLon));
            routeQuery.addWaypoint(QtPositioning.coordinate(endLat, endLon));
            routeModel.update();
        }

        onStatusChanged: {
            if (status === RouteModel.Ready && routeModel.count > 0) {
                routeItem.route = routeModel.get(0);
                calculateRemainingDistance();
            }
        }
    }

    function planRoute() {

        routeQuery.clearWaypoints();
        routeQuery.addWaypoint(QtPositioning.coordinate(startLat, startLon));
        routeQuery.addWaypoint(QtPositioning.coordinate(endLat, endLon));
        routeModel.update();
    }

    function calculateRemainingDistance() {
        remainingDistance = routeModel.get(0).distance;
    }



}
