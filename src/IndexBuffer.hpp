#pragma once

class IndexBuffer {
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    virtual ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int getID() const { return this->m_RendererID;}
    inline unsigned int getCount() const { return this->m_Count;}

private:
    unsigned int m_RendererID;
    unsigned int m_Count;
};
