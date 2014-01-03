import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle {
    id: picker

    property real hue: 0.0
    property real saturation: 0.0
    property real value: 0.0

    function color2hsv(color) {
        var r = color.r;
        var g = color.g;
        var b = color.b;

        var max = Math.max(r, g, b);
        var min = Math.min(r, g, b);
        var h, v = max;

        var d = max - min;
        var s = (max == 0) ? 0 : d / max;

        if (max == min) {
            h = 0;
        } else {
            switch (max) {
            case r: h = (g - b) / d + ((g < b) ? 6 : 0); break;
            case g: h = (b - r) / d + 2; break;
            case b: h = (r - g) / d + 4; break;
            }
            h = h / 6;
        }

        return { h: h, s: s, v: v };
    }

    function hsv2color(h, s, v) {
        var r, g, b;

        var i = Math.floor(h * 6);

        var f = h * 6 - i;
        var p = v * (1 - s);
        var q = v * (1 - f * s);
        var t = v * (1 - (1 - f) * s);

        switch (i % 6) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            case 5: r = v; g = p; b = q; break;
        }

        return Qt.rgba(r, g, b, 1.0);
    }

    color: hsv2color(hue, saturation, value)

    ShaderEffectSource {
        sourceItem: canvas
        hideSource: true
        anchors.fill: parent

        smooth: true
    }

    ShaderEffect {
        id: canvas

        width: parent.width * 2
        height: parent.height * 2

        property alias hue: picker.hue;
        property real radiusStart: 0.41
        property real radiusEnd: 0.49
        property real rectBorder: Math.sqrt(0.32) * 0.5

        fragmentShader: "varying highp vec2 qt_TexCoord0;
            uniform highp float hue;
            uniform highp float radiusStart;
            uniform highp float radiusEnd;
            uniform highp float rectBorder;

            vec3 hsv2rgb(vec3 c)
            {
                vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
                vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
                return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
            }

            vec4 convert_color(vec3 c)
            {
                return vec4(c.x, c.y, c.z, 1.0);
            }

            vec4 get_color(highp float h)
            {
                vec3 c = hsv2rgb(vec3(h, 1.0, 1.0));
                return convert_color(c);
            }

            void main(void)
            {
                vec2 c = vec2(qt_TexCoord0.y - 0.5, qt_TexCoord0.x - 0.5);
                highp float r = sqrt(c.x * c.x + c.y * c.y);
                highp float pi = 2.0 * acos(0.0);

                if ((abs(c.x) <= rectBorder) && (abs(c.y) <= rectBorder)) {
                    vec3 a = vec3(hue, c.y * 0.5 / rectBorder + 0.5, 0.5 - c.x * 0.5 / rectBorder);
                    gl_FragColor = convert_color(hsv2rgb(a));
                    return;
                }

                if ((r > radiusEnd) || (r < radiusStart)) {
                    gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
                    return;
                }

                highp float angle = atan(c.x, c.y) / pi * 0.5 + 0.25;

                gl_FragColor = get_color(angle);
            }"
    }

    Rectangle {
        x: centerX - width / 2
        y: centerY - width / 2

        property real angle: canvas.hue * 360
        property real radius: parent.width / 2 * (canvas.radiusEnd + canvas.radiusStart) / 2

        property real centerX: parent.width / 2 + 2 * radius * Math.cos(2 * angle / 360 * Math.PI - Math.PI / 2)
        property real centerY: parent.height / 2 + 2 * radius * Math.sin(2 * angle / 360 * Math.PI - Math.PI / 2)

        width: 20
        height: 20
        color: 'white'
    }

    Rectangle {
        x: centerX - width / 2
        y: centerY - width / 2

        property real border: canvas.rectBorder * parent.width * 2

        property real centerX: parent.width / 2 + border * (parent.saturation - 0.5)
        property real centerY: parent.height / 2 - border * (parent.value - 0.5)

        width: 20
        height: 20
        color: 'white'
    }

    MouseArea {
        id: mouse

        anchors.fill: parent
        preventStealing: true

        property int propertyType: -1

        function radiusForCoord(x, y) {
            return Math.sqrt(Math.pow(x, 2) + Math.pow(y, 2))
        }

        function angleForCoord(x, y) {
            return Math.atan2(y, x) / (Math.PI * 2);
        }

        function remapMouse(x, y) {
            return {
                x: (mouseX - (width / 2)) / width,
                y: (mouseY - (width / 2)) / width
            };
        }

        function normalize(x) {
            return Math.min(Math.max(x / canvas.rectBorder / 2 + 0.5, 0.0), 1.0);
        }

        function updatePosition(coords) {
            if (propertyType === 1) {
                var angle = angleForCoord(coords.x, coords.y) + 0.25;
                if (angle < 0)
                    angle += 1.0;
                picker.hue = angle;
            } else if (propertyType === 2) {
                picker.saturation = normalize(coords.x);
                picker.value = 1.0 - normalize(coords.y);
            }
        }

        onPressed: {
            var coords = remapMouse(mouseX, mouseY);
            var radius = radiusForCoord(coords.x, coords.y);

            if (radius >= canvas.radiusStart && radius <= canvas.radiusEnd) {
                propertyType = 1;
            } else if (Math.max(Math.abs(coords.x), Math.abs(coords.y)) <= canvas.rectBorder) {
                propertyType = 2;
            }

            updatePosition(coords);
        }

        onPositionChanged: {
            var coords = remapMouse(mouseX, mouseY);

            updatePosition(coords);
        }

        onReleased: {
            propertyType = -1;
        }
    }
}
