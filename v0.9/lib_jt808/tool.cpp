#include <execinfo.h>
#include <sys/time.h>
#include "tool.h"
extern char *program_name;
void error(int status, int err, char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	fprintf(stderr, "%s::",program_name);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if(err)
		fprintf(stderr, "error:: %s (%d)\n", strerror(errno), errno);

	void *trace[128];
	int trace_size = backtrace(trace, 128);
	char **messages = backtrace_symbols(trace, trace_size);
	fprintf(stderr, "Execution path:");
	for(int index=0; index<trace_size; ++index)
		fprintf(stderr, "%d %s\n", index, messages[index]);

	if(status)
		EXIT(status);
}
void split_string(const char * in, const char * split, char *** out, int *n_out)
{
	int split_len = strlen(split);
	char *copy_in = strdup(in), *dummy = copy_in;

	for(;;)
	{
		char *next = NULL;
		(*n_out)++;
		*out = reinterpret_cast<char **>(realloc(*out, *n_out * sizeof (char *)));
		next =strstr(copy_in,split);
		if(!next)
		{
			(*out)[*n_out - 1] = strdup(copy_in);	
			break;
		}

		*next = 0x00;
		(*out)[*n_out - 1] = strdup(copy_in);
		copy_in= next + split_len;
	}
	free (dummy);
}
std::vector<std::string> split_string(std::string in,std::string split)
{
	char **out = NULL;
	int n_out = 0;

	split_string(in.c_str(), split.c_str(), &out, &n_out);

	std::vector<std::string> list_out;
	for(int index=0;index< n_out;index++)
	{
		list_out.push_back(out[index]);
		free(out[index]);
	}
	free(out);
	return list_out;
}
std::string format(const char * fmt,...)
{
	char *buffer =NULL;
	va_list ap;

	va_start(ap,fmt);
	(void)vasprintf(&buffer, fmt, ap);
	va_end(ap);

	std::string result = buffer;
	free(buffer);
	return result ;
}
double get_ts()
{
	struct timeval ts;
	if(gettimeofday(&ts, NULL) == -1)
			error(1, 1, "get time error");
	printf("current time is %fs:%fvm\n",(double)ts.tv_sec ,(double)ts.tv_usec);
	return (double)ts.tv_sec + (double)ts.tv_usec/1000000;
}
