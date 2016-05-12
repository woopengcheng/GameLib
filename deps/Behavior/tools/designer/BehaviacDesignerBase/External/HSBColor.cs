////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009, Daniel Kollmann
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, this list of conditions
//   and the following disclaimer.
//
// - Redistributions in binary form must reproduce the above copyright notice, this list of
//   conditions and the following disclaimer in the documentation and/or other materials provided
//   with the distribution.
//
// - Neither the name of Daniel Kollmann nor the names of its contributors may be used to endorse
//   or promote products derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
// WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace TTRider.UI
{
    public struct HSBColor {
        float h;
        float s;
        float b;
        int a;

        public HSBColor(float h, float s, float b) {
            this.a = 0xff;
            this.h = Math.Min(Math.Max(h, 0), 255);
            this.s = Math.Min(Math.Max(s, 0), 255);
            this.b = Math.Min(Math.Max(b, 0), 255);
        }

        public HSBColor(int a, float h, float s, float b) {
            this.a = a;
            this.h = Math.Min(Math.Max(h, 0), 255);
            this.s = Math.Min(Math.Max(s, 0), 255);
            this.b = Math.Min(Math.Max(b, 0), 255);
        }

        public HSBColor(Color color) {
            HSBColor temp = FromColor(color);
            this.a = temp.a;
            this.h = temp.h;
            this.s = temp.s;
            this.b = temp.b;
        }

        public float H {
            get { return h; }
        }

        public float S {
            get { return s; }
        }

        public float B {
            get { return b; }
        }

        public int A {
            get { return a; }
        }

        public Color Color {
            get { return FromHSB(this); }
        }

        public static Color ShiftHue(Color c, float hueDelta) {
            HSBColor hsb = HSBColor.FromColor(c);
            hsb.h += hueDelta;
            hsb.h = Math.Min(Math.Max(hsb.h, 0), 255);
            return FromHSB(hsb);
        }

        public static Color ShiftSaturation(Color c, float saturationDelta) {
            HSBColor hsb = HSBColor.FromColor(c);
            hsb.s += saturationDelta;
            hsb.s = Math.Min(Math.Max(hsb.s, 0), 255);
            return FromHSB(hsb);
        }

        public static Color ShiftBrighness(Color c, float brightnessDelta) {
            HSBColor hsb = HSBColor.FromColor(c);
            hsb.b += brightnessDelta;
            hsb.b = Math.Min(Math.Max(hsb.b, 0), 255);
            return FromHSB(hsb);
        }

        public static Color FromHSB(HSBColor hsbColor) {
            float r = hsbColor.b;
            float g = hsbColor.b;
            float b = hsbColor.b;

            if (hsbColor.s != 0) {
                float max = hsbColor.b;
                float dif = hsbColor.b * hsbColor.s / 255f;
                float min = hsbColor.b - dif;

                float h = hsbColor.h * 360f / 255f;

                if (h < 60f) {
                    r = max;
                    g = h * dif / 60f + min;
                    b = min;

                } else if (h < 120f) {
                    r = -(h - 120f) * dif / 60f + min;
                    g = max;
                    b = min;

                } else if (h < 180f) {
                    r = min;
                    g = max;
                    b = (h - 120f) * dif / 60f + min;

                } else if (h < 240f) {
                    r = min;
                    g = -(h - 240f) * dif / 60f + min;
                    b = max;

                } else if (h < 300f) {
                    r = (h - 240f) * dif / 60f + min;
                    g = min;
                    b = max;

                } else if (h <= 360f) {
                    r = max;
                    g = min;
                    b = -(h - 360f) * dif / 60 + min;

                } else {
                    r = 0;
                    g = 0;
                    b = 0;
                }
            }

            return Color.FromArgb(hsbColor.a,
                                  (int)Math.Round(Math.Min(Math.Max(r, 0), 255)),
                                  (int)Math.Round(Math.Min(Math.Max(g, 0), 255)),
                                  (int)Math.Round(Math.Min(Math.Max(b, 0), 255)));
        }

        public static HSBColor FromColor(Color color) {
            HSBColor ret = new HSBColor(0f, 0f, 0f);
            ret.a = color.A;

            float r = color.R;
            float g = color.G;
            float b = color.B;

            float max = Math.Max(r, Math.Max(g, b));

            if (max <= 0)
            { return ret; }

            float min = Math.Min(r, Math.Min(g, b));
            float dif = max - min;

            if (max > min) {
                if (g == max) {
                    ret.h = (b - r) / dif * 60f + 120f;

                } else if (b == max) {
                    ret.h = (r - g) / dif * 60f + 240f;

                } else if (b > g) {
                    ret.h = (g - b) / dif * 60f + 360f;

                } else {
                    ret.h = (g - b) / dif * 60f;
                }

                if (ret.h < 0) {
                    ret.h = ret.h + 360f;
                }

            } else {
                ret.h = 0;
            }

            ret.h *= 255f / 360f;
            ret.s = (dif / max) * 255f;
            ret.b = max;

            return ret;
        }
    }
}
