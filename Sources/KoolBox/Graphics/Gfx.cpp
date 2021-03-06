#include "Gfx.h"
#include "Batcher.h"

namespace KoolBox
{

    Gfx::Gfx()
            : batcher(new Batcher()),
              program(nullptr)
    {
        assert(batcher);
    }

    Gfx::~Gfx()
    {
        delete batcher;
    }


    void Gfx::begin()
    {
        assert(batcher);
        batcher->begin();
    }

    void Gfx::end()
    {
        assert(batcher);
        batcher->end();
    }

    void Gfx::flush()
    {
        assert(batcher);
        batcher->flush();
    }

    //

    void Gfx::clear(u32 flags, u32 color, float depth, int stencil)
    {
        flush();
        Kore::Graphics4::clear(flags, color, depth, stencil);
    }

    void Gfx::setProgram(Program *newProgram)
    {
        assert(newProgram);
        assert(batcher);

        program = newProgram;
        batcher->set(program);
    }

    //

    void Gfx::setTexture(const char *name, Texture *tex, TextureFilter texFilter, TextureAddressing texAddressing)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getTextureUnit(name), tex, texFilter, texAddressing);
    }

    void Gfx::setFloat1(const char *name, float v0)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getConstant(name), v0);
    }

    void Gfx::setFloat2(const char *name, float v0, float v1)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getConstant(name), v0, v1);
    }

    void Gfx::setFloat3(const char *name, float v0, float v1, float v2)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getConstant(name), v0, v1, v2);
    }

    void Gfx::setFloat4(const char *name, float v0, float v1, float v2, float v3)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getConstant(name), v0, v1, v2, v3);
    }

    void Gfx::setFloats(const char *name, float *values, u32 size)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getConstant(name), values, size);
    }

    void Gfx::setMatrix(const char *name, Mat4 &matrix)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getConstant(name), matrix);
    }

    //

    void Gfx::setTexture(const u32 name, Texture *tex, TextureFilter texFilter, TextureAddressing texAddressing)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getTextureUnit(name), tex, texFilter, texAddressing);
    }

    void Gfx::setFloat1(const u32 name, float v0)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getConstant(name), v0);
    }

    void Gfx::setFloat2(const u32 name, float v0, float v1)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getConstant(name), v0, v1);
    }

    void Gfx::setFloat3(const u32 name, float v0, float v1, float v2)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getConstant(name), v0, v1, v2);
    }

    void Gfx::setFloat4(const u32 name, float v0, float v1, float v2, float v3)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getConstant(name), v0, v1, v2, v3);
    }

    void Gfx::setFloats(const u32 name, float *values, u32 size)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getConstant(name), values, size);
    }

    void Gfx::setMatrix(const u32 name, Mat4 &matrix)
    {
        assert(batcher);
        assert(program);
        batcher->set(program->getConstant(name), matrix);
    }

    //

    void Gfx::setTexture(TextureUnit unit, Texture *tex, TextureFilter texFilter, TextureAddressing texAddressing)
    {
        assert(batcher);
        batcher->set(unit, tex, texFilter, texAddressing);
    }

    void Gfx::setFloat1(ConstantLocation location, float v0)
    {
        assert(batcher);
        batcher->set(location, v0);
    }

    void Gfx::setFloat2(ConstantLocation location, float v0, float v1)
    {
        assert(batcher);
        batcher->set(location, v0, v1);
    }

    void Gfx::setFloat3(ConstantLocation location, float v0, float v1, float v2)
    {
        assert(batcher);
        batcher->set(location, v0, v1, v2);
    }

    void Gfx::setFloat4(ConstantLocation location, float v0, float v1, float v2, float v3)
    {
        assert(batcher);
        batcher->set(location, v0, v1, v2, v3);
    }

    void Gfx::setFloats(ConstantLocation location, float *values, u32 size)
    {
        assert(batcher);
        batcher->set(location, values, size);
    }

    void Gfx::setMatrix(ConstantLocation location, Mat4 &matrix)
    {
        assert(batcher);
        batcher->set(location, matrix);
    }

    //

    void Gfx::allocate(u16 iWant, u16 vWant, int **iPtr, float **vPtr, u16 *vPos)
    {
        assert(batcher);
        batcher->allocate(iWant, vWant, iPtr, vPtr, vPos);
    }

	u32 Gfx::getDrawCalls()
	{
		return batcher->drawCalls;
	}

    //

    void Gfx::draw(Sprite &item)
    {
        assert(batcher);
        assert(item.region);

        // get buffer space
        int   *ibp;
        float *vbp;
        u16    vbi;
        batcher->allocate(6, 4, &ibp, &vbp, &vbi);
        assert(ibp);
        assert(vbp);

        // get size of quad
        Rect &bounds = item.region->bounds;
        float w = bounds.w * item.scale.x;
        float h = bounds.h * item.scale.y;

        // get origin offsets
        float oox = w * item.origin.x;
        float ooy = h * item.origin.y;

        // get quad extents relative to origin
        // left, bottom, right, top
        float ql = -oox;
        float qb = -ooy;
        float qr = ql + w;
        float qt = qb + h;

        // construct corner points
        // 1-2
        // |/|
        // 0-3
        float x0, y0, x1, y1, x2, y2, x3, y3;

        // rotate
        if (item.rotation != 0)
        {
            float rad = DEG2RADF(item.rotation);
            float cos = KoolBox::cosa(rad);
            float sin = KoolBox::sina(rad);

            x0 = cos * ql - sin * qb;
            y0 = sin * ql + cos * qb;

            x1 = cos * ql - sin * qt;
            y1 = sin * ql + cos * qt;

            x2 = cos * qr - sin * qt;
            y2 = sin * qr + cos * qt;

            x3 = x0 + (x2 - x1);
            y3 = y2 - (y1 - y0);
        } else
        {
            x0 = ql;
            y0 = qb;

            x1 = ql;
            y1 = qt;

            x2 = qr;
            y2 = qt;

            x3 = qr;
            y3 = qb;
        }

        // get position of corner points in world space
        float px = item.position.x;
        float py = item.position.y;
        x0 += px;
        y0 += py;
        x1 += px;
        y1 += py;
        x2 += px;
        y2 += py;
        x3 += px;
        y3 += py;

        // get normalised texture coordinates
        Rect &uv = item.region->uv;
        float uvl = uv.x0;
        float uvb = uv.y0;
        float uvr = uv.x1;
        float uvt = uv.y1;

        if (item.flip.x)
        {
            float t = uvl;
            uvr = uvl;
            uvl = t;
        }

        if (item.flip.y)
        {
            float t = uvt;
            uvt = uvb;
            uvb = t;
        }

        // write index data to buffer
        // 1-2  1,2,0
        // |/|  0,2,3
        // 0-3
        *ibp++ = vbi + 1;
        *ibp++ = vbi + 2;
        *ibp++ = vbi;
        *ibp++ = vbi;
        *ibp++ = vbi + 2;
        *ibp = vbi + 3;

        // write vertex data to buffer
        // 1-2  1,2,0
        // |/|  0,2,3
        // 0-3
        float *cp = item.colours.f;

        *vbp++ = x0;
        *vbp++ = y0;
        *vbp++ = uvl;
        *vbp++ = uvb;
        *vbp++ = *cp++;

        *vbp++ = x1;
        *vbp++ = y1;
        *vbp++ = uvl;
        *vbp++ = uvt;
        *vbp++ = *cp++;

        *vbp++ = x2;
        *vbp++ = y2;
        *vbp++ = uvr;
        *vbp++ = uvt;
        *vbp++ = *cp++;

        *vbp++ = x3;
        *vbp++ = y3;
        *vbp++ = uvr;
        *vbp++ = uvb;
        *vbp = *cp;
    }

    void Gfx::draw(Text &item)
    {
        // initialise
        BitmapFont *font = item.font;
        assert(font);
        const char *text = item.text;
        assert(text);

        // count how many characters will actually be rendered
        u16 textlen = (u16) strlen(text);
        u16 textbuf = textlen;
        for (int i = 0; i < textlen; i++)
        {
            BitmapFont::Char *cdef = font->getCharDef(text[i]);
            if (cdef->w == 0 || cdef->h == 0)
            {
                textbuf--;
            }
        }
        if (textbuf <= 0) {return;}   // just in case :D

        // get buffer space for characters
        int   *ibp;
        float *vbp;
        u16    vbi;
        batcher->allocate((u16) 6 * textbuf, (u16) 4 * textbuf, &ibp, &vbp, &vbi);
        assert(ibp);
        assert(vbp);

        // initialise part 2
        float scalex = item.scale.x;
        float scaley = item.scale.y;
        Size bounds = font->getBounds(text, item.scale);
        float x = item.position.x - (item.origin.x * bounds.w);
        float y = item.position.y - (item.origin.y * bounds.h);
        float col0 = item.colours.f[0];
        float col1 = item.colours.f[1];
        float col2 = item.colours.f[2];
        float col3 = item.colours.f[3];

        // render characters
        for (int i = 0; i < textlen; i++)
        {
            // find the next Char entry
            BitmapFont::Char *cdef = font->getCharDef(text[i]);
            if (cdef->w > 0 && cdef->h > 0)
            {
                float l = x + (cdef->xOffset * scalex);
                float b = y + (cdef->yOffset * scaley);
                float r = l + (cdef->xSize * scalex);
                float t = b + (cdef->ySize * scaley);

                float ul = cdef->u;
                float vb = cdef->v;
                float ur = ul + cdef->w;
                float vt = vb + cdef->h;

                // write index data to buffer
                // 1-2  1,2,0
                // |/|  0,2,3
                // 0-3
                *ibp++ = vbi + 1;
                *ibp++ = vbi + 2;
                *ibp++ = vbi;
                *ibp++ = vbi;
                *ibp++ = vbi + 2;
                *ibp++ = vbi + 3;
                vbi += 4;

                // write vertex data to buffer
                // 1-2  1,2,0
                // |/|  0,2,3
                // 0-3
                //
                // 1 (TL)
                *vbp++ = l;    // pos
                *vbp++ = t;
                *vbp++ = ul;   // tex
                *vbp++ = vb;
                *vbp++ = col1;

                // 2 (TR)
                *vbp++ = r;
                *vbp++ = t;
                *vbp++ = ur;
                *vbp++ = vb;
                *vbp++ = col2;

                // 3 (BR)
                *vbp++ = r;
                *vbp++ = b;
                *vbp++ = ur;
                *vbp++ = vt;
                *vbp++ = col3;

                // 0 (BL)
                *vbp++ = l;
                *vbp++ = b;
                *vbp++ = ul;
                *vbp++ = vt;
                *vbp++ = col0;
            }

            // step cursor to next character position
            x += cdef->xAdvance * scalex;
        }
    }

}