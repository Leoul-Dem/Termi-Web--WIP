#include <string>
#include <vector>
#include "gumbo.h"
#include <vector>

int parse_page(const std::string& url,
                std::string& page,
                std::vector<std::string>& fwd_links){
    cpr::Response resp = cpr::Get(cpr::Url{url});

    if(resp.status_code != 200){
        std::cerr << "HTTP request failed. Code: " << resp.status_code << std::endl;
        return 1;
    }

    GumboOutput* output = gumbo_parse(resp.text.c_str());
    if(!output){
        std::cerr << "Gumbo failed to parse HTML" << std::endl;
        return 2;
    }

    gather_links(output->root, fwd_links);

    std::stringstream ss;
    extract_text(output->root, ss);
    page = ss.str();

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    return 0;
}