struct sub_input {
	char s[10];
	float *f_sub;
	struct sub_input *self;
	char padding[4096]; // Page off just for checking, don't copy this
	int q;
};

struct lib_input {
    int *i_ptr;
    double *d_ptr;
    float f;
	struct sub_input sub; /* we need to handle substructs :/ */
	struct sub_input *sub_ptr;
	int (*func_ptr)(double, int *);
};

struct lib_output {
	int i;
	int *i_ptr;
	double *d_ptr;
	char *s_ptr;
	char s[10];
};
