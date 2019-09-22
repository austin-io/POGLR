#pragma once

class VertexBuffer {
public:
    VertexBuffer(const void* data, unsigned int size);
    virtual ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    void setData(const void* data, unsigned int size);

    inline unsigned int getID(){ return this->m_RendererID;}

private:
    unsigned int m_RendererID;
};