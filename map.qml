import QtQuick 2.0
import QtLocation
import QtPositioning


Rectangle{
    Plugin{
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: mapView
        anchors.fill: parent
        plugin: mapPlugin
        center : QtPositioning.coordinate(25,6585,125.3658);
        zoomlevel: 15

    }
}
