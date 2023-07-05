#ifndef CPP_H
# define CPP_H
# include <stdlib.h>
# include <libc.h>
# include <ctype.h>
# include <dlfcn.h>
# include <errno.h>
# define ULL unsigned long long
# define LL long long# define ull unsigned long long
# define STR(...) #__VA_ARGS__
# define ALLOC(TYPE, ...) ({					\
	TYPE *o = malloc(sizeof(TYPE));				\
    if (!o)                                     \
    {                                           \
        printf("error, allocation error at %s:%i\n", __FILE__, __LINE__);\
        exit(1);                                \
        o;                                      \
    }                                           \
	*o = (TYPE) {__VA_ARGS__};				    \
	o;										    \
})
#define print(...) 								\
	(fprintf(stderr, "[%s:%i]\t", __FILE__, __LINE__)\
+ 												\
	fprintf(stderr, __VA_ARGS__))
# define DEF_LIST_PROTO(TYPE, NAME)     \
                                        \
	typedef struct s_ ## NAME			\
	{									\
		TYPE				data;		\
		struct s_ ## NAME	*next;		\
		struct s_ ## NAME	*prev;		\
	}  NAME;							\
NAME    *NAME ## _last(NAME *l);		\
NAME    *NAME ## _new(TYPE data);		\
NAME    *NAME ## _add(NAME **l, TYPE data);\
NAME    *NAME ## _link(NAME **l, NAME *i);\
void    NAME ## _del(NAME **l, NAME *k);\
void    NAME ## _free(NAME *l);         \
NAME    *NAME ## _clone(NAME    *, TYPE (*)(TYPE));		\

# define DEF_LIST(TYPE, NAME, FREEF)	\
										\
NAME    *NAME ## _last(NAME *l)			\
{										\
    while (l)							\
    {									\
        if (!l->next)					\
            return l;					\
        l = l->next;					\
    }									\
    return 0;							\
}										\
										\
NAME    *NAME ## _new(TYPE data)		\
{										\
    NAME *o;							\
										\
    o = malloc(sizeof(NAME));			\
    o->data = data;						\
    o->next = 0;						\
    o->prev = 0;                        \
    return o;							\
}										\
										\
NAME    *NAME ## _add(NAME **l, TYPE data)\
{										\
    NAME *last;							\
    NAME *prev;							\
                                        \
    prev = 0;                           \
    last = *l;							\
    while (last)						\
    {									\
        if (!last->next)				\
            break ;                     \
        prev = last;                    \
        last = last->next;				\
    }	                                \
    if (!last)							\
    {                                   \
        return *l = NAME ## _new(data);	\
    }                                   \
    last->next = NAME ## _new(data);    \
    last->next->prev = last;            \
                                        \
    return last->next;                  \
                                        \
}                                       \
										\
NAME    *NAME ## _link(NAME **l, NAME *i)\
{										\
    NAME *last;							\
    NAME *prev;							\
                                        \
    prev = 0;                           \
    last = *l;							\
    while (last)						\
    {									\
        if (!last->next)				\
            break ;                     \
        prev = last;                    \
        last = last->next;				\
    }	                                \
    if (!last)							\
    {                                   \
		i->prev = 0;					\
        return *l = i;					\
    }                                   \
    last->next = i; 					\
    last->next->prev = last;            \
                                        \
    return last->next;                  \
                                        \
}                                       \
										\
void   NAME ## _del(NAME **l, NAME *k)  \
{										\
    NAME    *it;						\
    NAME    *swp;						\
    NAME    **prev;						\
										\
    it = *l;							\
    prev = l;							\
    while (it)							\
    {									\
        swp = it->next;					\
        if (it == k)					\
        {								\
            if (FREEF)		            \
                ((void(*)(void*))FREEF)((void*)(unsigned long long)it->data);\
            free(it);					\
            *prev = swp;				\
        }								\
        it = swp;						\
        if (it)							\
            prev = &(it->next);			\
    }									\
}										\
										\
void    NAME ## _free(NAME *l)          \
{										\
    NAME    *it;						\
    NAME    *swp;						\
										\
    it = l;								\
    while (it)							\
    {									\
        swp = it->next;					\
        if (FREEF)			            \
                ((void(*)(void*))FREEF)((void*)(unsigned long long)it->data);\
        free(it);						\
        it = swp;						\
    }									\
}										\
                                        \
NAME    * NAME ## _clone(NAME *l, TYPE (*clone)(TYPE))\
{										\
    NAME    *it;						\
    NAME    *out = 0;					\
										\
    it = l;								\
    while (it)							\
    {									\
		NAME ## _add(&out, it->data);	\
        it = it->next;					\
    }									\
    return out;                         \
}

char	*read_file(const char *path)
{
	int     fd;
	size_t  fsize;
	char	*src;
	ssize_t	bytes;
	char	*ret;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		print("ERROR: open=%i for %s\n", errno, path);
		return 0;
	}
	fsize = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	src = malloc(fsize + 1);
	if (!src)
	{
		close(fd);
		return 0;
	}
	src[fsize] = 0;
	fsize = 0;
	while((bytes = read(fd, src + fsize++, 1)) > 0)
		;
	close(fd);
	if (bytes != 0)
	{
		print("ERROR: read=%i for %s\n", errno, path);
		free(src);
		return 0;
	}
	return src;
}

char* findLastUnescapedQuote(const char* str) {
    int len = strlen(str);
    int i = len - 1;

    while (i >= 0) {
        if (str[i] == '"' && (i == 0 || str[i - 1] != '\\')) {
            return (char*)&str[i];
        }
        i--;
    }

    return NULL;
}

#endif
