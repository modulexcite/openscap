/**
 * @file cpedict.c
 * \brief Interface to Common Platform Enumeration (CPE) Dictionary.
 *
 * See more details at http://nvd.nist.gov/cpe.cfm
 */

/*
 * Copyright 2008 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors:
 *      Maros Barabas <mbarabas@redhat.com>
 *      Lukas Kuklinek <lkuklinek@redhat.com>
 */

#include "cpedict.h"
#include "../common/list.h"
#include "../common/util.h"

struct cpe_dict * cpe_dict_import(const char *fname) {

    if (fname == NULL) return NULL;

    return cpe_dict_parse(fname);
}

bool cpe_name_match_dict(struct cpe_name * cpe, struct cpe_dict * dict) {

	if (cpe == NULL || dict == NULL)
		return false;
	
    struct cpe_item_iterator *items = cpe_dict_get_items(dict);
	size_t n = oscap_iterator_get_itemcount((struct oscap_iterator *)items);
	struct cpe_name** cpes = oscap_alloc(sizeof(struct cpe_name*) * n);
	//struct oscap_list_item* cur = ((struct oscap_list *) cpe_dict_get_items(dict))->first;

	int i = 0;
	OSCAP_FOREACH (cpe_item, item, items,
		cpes[i++] = cpe_item_get_name(item);
	)
	
	bool ret = cpe_name_match_cpes(cpe, n, cpes);

	oscap_free(cpes);

	return ret;
}

bool cpe_name_match_dict_str(const char *cpestr, struct cpe_dict * dict)
{
	bool ret;
	if (cpestr == NULL)
		return false;
	struct cpe_name *cpe = cpe_name_new(cpestr);
	if (cpe == NULL) return false;
	ret = cpe_name_match_dict(cpe, dict);
	cpe_name_free(cpe);
	return ret;
}
