// #include <stdio.h>
// #include <string.h>

#include <map>
#include <utility>
#include <algorithm>

// #include "lib.h"
// #include "generated.h"



struct cls_less {
  OBJ (*fn)(OBJ, OBJ, const OBJ *, const generated::CLS *, generated::ENV &env);
  const OBJ *data;
  const generated::CLS *cls;
  generated::ENV &env;

  cls_less(
    OBJ (*fn)(OBJ, OBJ, const OBJ *, const generated::CLS *, generated::ENV &env),
    const OBJ *data,
    const generated::CLS *cls,
    generated::ENV &env
  ) : fn(fn), data(data), cls(cls), env(env) {}

  bool operator () (const OBJ &a, const OBJ &b) {
    return get_bool(fn(a, b, data, cls, env));
  }
};


struct obj_pair {
  OBJ obj1;
  OBJ obj2;
};


bool operator < (const obj_pair &p1, const obj_pair &p2) {
  int res = comp_objs(p1.obj1, p2.obj1);
  if (res == 0)
    res = comp_objs(p1.obj2, p2.obj2);
  return res > 0;
}


struct obj_triple {
  OBJ obj1;
  OBJ obj2;
  OBJ obj3;
};


namespace generated {
  // OBJ expr_type_F2_(OBJ p0, OBJ p1, ENV &env);
  // OBJ typecheck_function_F1_(OBJ p0, ENV &env);

  OBJ source_file_location_F1_(OBJ p0, ENV &env);
  OBJ build_amber_file_ast_F3_(OBJ p0, OBJ p1, OBJ p2, ENV &env);

  OBJ sort_F2_(OBJ p0, CLS &c_lt, ENV &env);

  //////////////////////////////////////////////////////////////////////////////

  typedef std::map<void *, obj_pair> ptr_to_obj_pair_t;

  static ptr_to_obj_pair_t source_file_location_cache;

  OBJ source_file_location_F1(OBJ p0, ENV &env) {
    if (is_inline_obj(p0))
      return source_file_location_F1_(p0, env);

    REF_OBJ *ptr = get_ref_obj_ptr(p0);
    ptr_to_obj_pair_t::iterator it = source_file_location_cache.find(ptr);
    if (it != source_file_location_cache.end()) {
      OBJ cached_res = it->second.obj2;
      add_ref(cached_res);
      return cached_res;
    }

    OBJ res = source_file_location_F1_(p0, env);

    if (source_file_location_cache.find(ptr) == source_file_location_cache.end()) {
      // The result may have already been cached inside the previous call to source_file_location()
      // If that happens, we have to be careful with the garbage collector

      obj_pair value;
      value.obj1 = p0;
      value.obj2 = res;

      source_file_location_cache[ptr] = value;

      add_ref(p0);
      add_ref(res);
    }

    return res;
  }

  OBJ build_amber_file_ast_F3(OBJ p0, OBJ p1, OBJ p2, ENV &env) {
    OBJ res = build_amber_file_ast_F3_(p0, p1, p2, env);

    //## DOING THE GARBAGE COLLECTION HERE TAKES MORE THAN 300ms. WHY IS IT SO SLOW?
    for (ptr_to_obj_pair_t::iterator it = source_file_location_cache.begin() ; it != source_file_location_cache.end() ; it++) {
      release(it->second.obj1);
      release(it->second.obj2);
    }

    source_file_location_cache.clear();

    return res;
  }

  //////////////////////////////////////////////////////////////////////////////

  // typedef std::map<obj_pair, obj_triple> expr_type_cache_t;

  // static expr_type_cache_t expr_type_cache;

  // OBJ expr_type_F2(OBJ expr, OBJ var_env, ENV &env) {
  //   return expr_type_F2_(expr, var_env, env);
  // }


  // OBJ expr_type_F2(OBJ expr, OBJ var_env, ENV &env) {
  //   if (is_inline_obj(expr))
  //     return expr_type_F2_(expr, var_env, env);

  //   obj_pair key;
  //   key.obj1 = expr;
  //   key.obj2 = var_env;

  //   // uint64 key = (uint64) get_ref_obj_ptr(expr);
  //   // if (is_ref_obj(var_env))
  //   //   key += (uint64) get_ref_obj_ptr(var_env);

  //   expr_type_cache_t::iterator it = expr_type_cache.find(key);
  //   if (it != expr_type_cache.end()) {
  //     // if (comp_objs(it->second.obj1, expr) == 0 && comp_objs(it->second.obj2, var_env) == 0)
  //     // if (are_shallow_eq(it->second.obj1, expr) && are_shallow_eq(it->second.obj2, var_env))

  //     OBJ cached_res = it->second.obj3;
  //     add_ref(cached_res);
  //     return cached_res;
  //   }

  //   OBJ res = expr_type_F2_(expr, var_env, env);

  //   if (expr_type_cache.find(key) == expr_type_cache.end()) {
  //     // The result may have already been cached inside the previous call to expr_type()
  //     // If that happens, we have to be careful with the garbage collector

  //     obj_triple value;
  //     value.obj1 = expr;
  //     value.obj2 = var_env;
  //     value.obj3 = res;

  //     expr_type_cache[key] = value;

  //     add_ref(expr);
  //     add_ref(var_env);
  //     add_ref(res);
  //   }

  //   return res;
  // }

  // OBJ typecheck_function_F1(OBJ p0, ENV &env) {
  //   OBJ res = typecheck_function_F1_(p0, env);

  //   for (expr_type_cache_t::iterator it = expr_type_cache.begin() ; it != expr_type_cache.end() ; it++) {
  //     release(it->second.obj1);
  //     release(it->second.obj2);
  //     release(it->second.obj3);
  //   }
  //   expr_type_cache.clear();

  //   return res;
  // }

  //////////////////////////////////////////////////////////////////////////////

  OBJ sort_F2(OBJ seq, const CLS &c_lt, ENV &env) {
    if (is_empty_seq(seq))
      return make_empty_seq();

    int len = get_seq_length(seq);
    OBJ *in_elems = get_seq_buffer_ptr(seq);

    // SEQ_OBJ *out_seq = new_seq(len);
    // OBJ *out_elems = out_seq->buffer;
    OBJ out_seq = make_seq(new_seq(len), len);
    OBJ *out_elems = get_seq_buffer_ptr(out_seq);

    memcpy(out_elems, in_elems, len * sizeof(OBJ));
    vec_add_ref(out_elems, len);

    cls_less cmp(c_lt.fn_ptr_2, c_lt.data, c_lt.cls, env);

    std::sort(out_elems, out_elems+len, cmp);

    // return make_seq(out_seq, len);
    return out_seq;
  }

  //////////////////////////////////////////////////////////////////////////////

  static std::map<REF_OBJ*, OBJ> attachments_map;

  OBJ attach_F2(OBJ target_obj, OBJ data, ENV &env) {
    if (is_ref_obj(target_obj)) {
      REF_OBJ *target_ptr = get_ref_obj_ptr(target_obj);
      // std::map<REF_OBJ*, OBJ>::iterator it = attachments_map.find(target_ptr);
      add_ref(target_obj);
      add_ref(data);
      add_obj_to_cache(target_obj);
      add_obj_to_cache(data);
      attachments_map[target_ptr] = data;
    }
    add_ref(target_obj);
    return target_obj;
  }

  OBJ fetch_F1(OBJ obj, ENV &env) {
    if (is_ref_obj(obj)) {
      REF_OBJ *ptr = get_ref_obj_ptr(obj);
      std::map<REF_OBJ*, OBJ>::iterator it = attachments_map.find(ptr);
      if (it != attachments_map.end()) {
        OBJ res = it->second;
        add_ref(res);
        return make_tag_obj(symb_idx_just, res);
      }
    }
    return make_symb(symb_idx_nothing);
  }
}
