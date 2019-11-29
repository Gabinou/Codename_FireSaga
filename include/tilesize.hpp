#ifndef TILESIZE_HPP
#define TILESIZE_HPP

class Tilesize {

    public:
        Tilesize();
        ~Tilesize();
        Tilesize(const short int unsigned width, const short int unsigned height);
        void setTilesize(const short int unsigned width, const short int unsigned height);
        int * getTilesize() const;

    private:
        int tilesize[2];

};

#endif /* TILESIZE_HPP */