#ifndef _FUNC
#define _FUNC



int IsBind(in Texture2D _tex)
{
    uint width = 0;
    uint height = 0;
    _tex.GetDimensions(width, height);
    
    if (0 != width || 0 != height)
        return 1;
   
    return 0;
}


#endif