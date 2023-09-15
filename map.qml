import QtQuick 2.0
import QtLocation 5.11
import QtPositioning 5.11


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
