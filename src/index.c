#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

struct blog_entry {
	char *subject;
	char *content;
	char *date;
};

struct blog_entries {
	struct blog_entry *entries;
	int length;
};

static int compare_files(const void *left, const void *right) {
	struct stat lbuf, rbuf;
	struct dirent *l = *(struct dirent **) left;
	struct dirent *r = *(struct dirent **) right;
	char *filename = malloc(16 + strlen(l->d_name));
	snprintf(filename, 16 + strlen(l->d_name), "./include/blog/%s", l->d_name);
	stat(filename, &lbuf);
	free(filename);
	filename = malloc(16 + strlen(r->d_name));
	snprintf(filename, 16 + strlen(r->d_name), "./include/blog/%s", r->d_name);
	stat(filename, &rbuf);

	if(lbuf.st_mtime < rbuf.st_mtime)
		return 1;
	if(rbuf.st_mtime < lbuf.st_mtime)
		return -1;

	return 0;
}

struct blog_entries get_blog_entries(void) {
	DIR *blog_dir = opendir("./include/blog");
	struct dirent **files = calloc(1024, sizeof(struct dirent *));
	int i = 0;

	while((files[i] = readdir(blog_dir)))
		if(files[i]->d_type == DT_REG && strcasestr(files[i]->d_name, ".entry") != NULL)
			i++;

	qsort(files, i, sizeof(struct dirent **), compare_files);

	struct blog_entry *blog_entries = calloc(i, sizeof(struct blog_entry));
	struct blog_entries entries = { blog_entries, i };
	int i1 = 0;
	for(int i2 = 0; i2 < i; i2++) {
		struct blog_entry entry;

		char *filename = malloc(16 + strlen(files[i2]->d_name));
		snprintf(filename, 16 + strlen(files[i2]->d_name), "./include/blog/%s", files[i2]->d_name);
		FILE *fp = fopen(filename, "r");
		char c;
		size_t content_length = 0;
		size_t subject_length = 0;

		while((c = fgetc(fp)) != EOF) {
			if(c == '\n') {
				if(subject_length == 0) {
					subject_length += ftell(fp) - 1;
					content_length -= subject_length;
				} else
					content_length += 3;
			}
			content_length++;
		}
		subject_length++;
		content_length++;
		rewind(fp);

		if(content_length == 0) {
			fclose(fp);
			continue;
		}

		entry.subject = malloc(subject_length);
		entry.content = malloc(content_length);
		entry.date = malloc(22);
		struct stat s;
		if(!stat(filename, &s))
			strftime(entry.date, 22, "%Y-%m-%d %H:%M:%S", localtime(&s.st_mtime));
		else {
			fclose(fp);
			continue;
		}

		entry.subject = fgets(entry.subject, subject_length, fp);
		fgetc(fp);

		int i3 = 0;
		while((c = fgetc(fp)) != EOF) {
			entry.content[i3++] = c;
			if(c == '\n') {
				entry.content[i3++] = '\t';
				entry.content[i3++] = '\t';
				entry.content[i3++] = '\t';
			}
		}
		entry.content[i3] = '\0';
		fclose(fp);

		blog_entries[i1++] = entry;
	}

	return entries;
}

int main(void) {
	clock_t begin, end;
	double time_spent;
	begin = clock();
	printf("Content-type: text/html;charset=UTF-8\n\n");
	printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n");
	printf("<html>\n");
	printf("\t<head>\n");
	printf("\t\t<title>Sanctuary of Slack</title>\n");
	printf("\t\t<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
	printf("\t\t<meta http-equiv=\"pragma\" content=\"no-cache\">\n");
	printf("\t\t<meta http-equiv=\"expires\" content=\"-1\">\n");
	printf("\t\t<meta name=\"keywords\" content=\"slackware, slack, ware, church, of, bob, slacky, programming, libslack, libslack.so, pharmaceuticals, meds, medications, drugs\">\n");
	printf("\t\t<meta name=\"description\" content=\"Sanctuary of Slack\">\n");
	printf("\t\t<link rel=\"stylesheet\" type=\"text/css\" href=\"style/index.css\" media=\"screen\">\n");
	printf("\t</head>\n");
	printf("\t<body>\n");
	printf("\t\t<h1>Sanctuary of Slack</h1>\n");
	printf("\t\t<h5>We are very rare and mostly men.</h5>\n");
	printf("\t\t<p>Slack(n.): The state in which you need nothing, because you already have it.<br>-- Dobbs, 1956</p>\n");
	struct blog_entries blog_entries = get_blog_entries();
	for(int i = 0; i < blog_entries.length; i++) {
		if(blog_entries.entries[i].content) {
			printf("\t\t<div class=\"entry\">\n");
			printf("\t\t\t<div class=\"subject\">%s</div><br>\n", blog_entries.entries[i].subject);
			printf("\t\t\t<div class=\"content\">%s</div><br>\n", blog_entries.entries[i].content);
			printf("\t\t\t<div class=\"date\">%s</div><br>\n", blog_entries.entries[i].date);
			printf("\t\t</div>\n");
		}
	}
	printf("\t\t<div id=\"footer\">\n");
	printf("\t\t\t<a href=\"http://validator.w3.org/check?uri=referer\"><img src=\"http://www.w3.org/Icons/valid-html401\" alt=\"Valid HTML 4.01 Strict\" height=\"31\" width=\"88\"></a>\n");
	printf("\t\t\t<a href=\"http://jigsaw.w3.org/css-validator/check/referer\"><img style=\"border:0;width:88px;height:31px\" src=\"http://jigsaw.w3.org/css-validator/images/vcss\" alt=\"Valid CSS!\"></a><br>\n");
	end = clock();
	time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("Execution time: %.3f\n", time_spent);
	printf("\t\t</div>\n");
	printf("\t</body>\n");
	printf("</html>");
	return 0;
}
