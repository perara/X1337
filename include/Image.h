//
// Created by per on 4/20/21.
//

#ifndef X1337_IMAGE_H
#define X1337_IMAGE_H

class Image {
public:
    Image(size_t rows,
           size_t cols,
           size_t depth,
           unsigned char* data
           )
           : m_rows(rows)
           , m_cols(cols)
           , m_depth(depth)
           , m_data(data)
           {}

    unsigned char *data() { return m_data; }
    size_t rows() const { return m_rows; }
    size_t cols() const { return m_cols; }
    size_t channels() const { return m_depth; }
private:
    size_t m_rows, m_cols, m_depth;
    unsigned char* m_data;
};


#endif //X1337_IMAGE_H
