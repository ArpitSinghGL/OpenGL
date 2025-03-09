bool load_raw_rgb_pixel_data(int* width , int* height , unsigned char** data)
{
    *width = 100;
    *height = 100;

    *data = new unsigned char[100 * 100 * 3];

    auto ptr = *data;

    for(int y = 0 ; y < 100 ; y++)
    {
        for(int x = 0 ; x < 100 ; x++)
        {
            ptr[y * 100 * 3 + 3 * x + 0] = 0xff;
            ptr[y * 100 * 3 + 3 * x + 1] = 0x00;
            ptr[y * 100 * 3 + 3 * x + 2] = 0x00;
        }
    }

    return true;
}